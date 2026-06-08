"""
api.py — FastAPI pricing server.

Endpoints:
    GET  /health
    POST /price/black-scholes
    POST /price/heston
    GET  /greeks

Start with:
    conda activate finance
    export PYTHONPATH="$(pwd)/build:$PYTHONPATH"
    uvicorn api:app --reload --port 8000

Or:
    python api.py
"""

from __future__ import annotations

import math
import os
import sys
from pathlib import Path
from typing import Literal, Optional

from fastapi import FastAPI, HTTPException, Query
from fastapi.responses import JSONResponse, FileResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel, Field, model_validator

import pricing as _pricing
import market_data as _md

app = FastAPI(
    title="Financial Pricing API",
    description="Black-Scholes and Heston option pricing backed by a C++ core.",
    version="0.1.0",
)

# Serve static files (dashboard) from the project root
_ROOT = Path(__file__).parent
app.mount("/static", StaticFiles(directory=str(_ROOT)), name="static")


# ---------------------------------------------------------------------------
# Shared helpers
# ---------------------------------------------------------------------------

def _check_core() -> None:
    if not _pricing._CORE_AVAILABLE:
        raise HTTPException(
            status_code=503,
            detail="pricing_core C++ module not loaded. Build it with ./build_bridge.sh.",
        )


def _nan_to_none(v: float) -> Optional[float]:
    return None if math.isnan(v) else round(v, 8)


# ---------------------------------------------------------------------------
# Request / Response models
# ---------------------------------------------------------------------------

class OptionParamsBase(BaseModel):
    """
    Common option parameters.
    Either provide raw numbers OR a ticker (spot and risk_free_rate fetched live).
    """
    ticker: Optional[str] = Field(
        None,
        description="If set, spot_price and risk_free_rate are fetched from yFinance.",
        examples=["SPY"],
    )
    spot_price: Optional[float] = Field(None, gt=0, description="Underlying spot price.", examples=[100.0])
    strike_price: float         = Field(..., gt=0, description="Option strike price.",    examples=[100.0])
    time_to_expiration: float   = Field(..., gt=0, description="Time to expiry in years (e.g. 0.5).", examples=[1.0])
    risk_free_rate: Optional[float] = Field(None, description="Continuously compounded rate (e.g. 0.05). Fetched if omitted.", examples=[0.05])
    volatility: float           = Field(..., gt=0, lt=1, description="Annualised volatility in (0, 1).", examples=[0.2])
    option_type: Literal["call", "put"] = Field("call", description="Option type.")

    @model_validator(mode="after")
    def _resolve_spot(self) -> "OptionParamsBase":
        if self.spot_price is None and self.ticker is None:
            raise ValueError("Provide either 'ticker' or 'spot_price'.")
        if self.ticker and self.spot_price is None:
            snap = _md.get_market_snapshot(self.ticker)
            self.spot_price = snap.spot
            if self.risk_free_rate is None:
                self.risk_free_rate = snap.risk_free_rate
        if self.risk_free_rate is None:
            self.risk_free_rate = _md.get_risk_free_rate()
        return self


class HestonParams(OptionParamsBase):
    v0:    float = Field(..., ge=0,   description="Initial variance (v0 = σ₀²).",          examples=[0.04])
    kappa: float = Field(..., gt=0,   description="Mean-reversion speed.",                  examples=[2.0])
    theta: float = Field(..., gt=0,   description="Long-run mean variance.",                examples=[0.04])
    sigma: float = Field(..., gt=0,   description="Volatility of variance (vol-of-vol).",   examples=[0.3])
    rho:   float = Field(..., ge=-1, le=1, description="Spot-variance correlation.",        examples=[-0.7])
    use_monte_carlo: bool = Field(True,  description="True = Monte Carlo, False = analytic characteristic fn.")
    num_simulations: int  = Field(10_000, ge=100, le=500_000, description="MC paths (ignored when analytic).")
    num_time_steps:  int  = Field(100,    ge=10,  le=10_000,  description="Discretisation steps per path.")


class BSPriceResponse(BaseModel):
    model: str = "black-scholes"
    option_type: str
    spot_price: float
    strike_price: float
    time_to_expiration: float
    risk_free_rate: float
    volatility: float
    price: Optional[float]
    d1: Optional[float]
    d2: Optional[float]


class HestonPriceResponse(BaseModel):
    model: str = "heston"
    option_type: str
    spot_price: float
    strike_price: float
    time_to_expiration: float
    risk_free_rate: float
    volatility: float
    v0: float
    kappa: float
    theta: float
    sigma: float
    rho: float
    method: str
    price: Optional[float]


class GreeksResponse(BaseModel):
    model: str = "black-scholes"
    option_type: str
    spot_price: float
    strike_price: float
    time_to_expiration: float
    risk_free_rate: float
    volatility: float
    delta: Optional[float]
    gamma: Optional[float]
    vega: Optional[float]
    theta: Optional[float]
    rho: Optional[float]


# ---------------------------------------------------------------------------
# Routes
# ---------------------------------------------------------------------------

@app.get("/health", tags=["meta"])
def health() -> dict:
    """Returns service status and whether the C++ core is loaded."""
    return {
        "status": "ok",
        "pricing_core_loaded": _pricing._CORE_AVAILABLE,
    }


@app.post("/price/black-scholes", response_model=BSPriceResponse, tags=["pricing"])
def price_black_scholes(params: OptionParamsBase) -> BSPriceResponse:
    """
    Price a European option using the Black-Scholes model.

    Pass either raw parameters or a `ticker` to fetch live spot/rate.
    """
    _check_core()
    opt_type = _pricing.OptionType.CALL if params.option_type == "call" else _pricing.OptionType.PUT

    result = _pricing.price_black_scholes(
        underlying_price=params.spot_price,
        strike_price=params.strike_price,
        time_to_expiration=params.time_to_expiration,
        risk_free_rate=params.risk_free_rate,
        volatility=params.volatility,
        option_type=opt_type,
    )

    return BSPriceResponse(
        option_type=params.option_type,
        spot_price=params.spot_price,
        strike_price=params.strike_price,
        time_to_expiration=params.time_to_expiration,
        risk_free_rate=params.risk_free_rate,
        volatility=params.volatility,
        price=_nan_to_none(result.price),
        d1=_nan_to_none(result.d1),
        d2=_nan_to_none(result.d2),
    )


@app.post("/price/heston", response_model=HestonPriceResponse, tags=["pricing"])
def price_heston(params: HestonParams) -> HestonPriceResponse:
    """
    Price a European option using the Heston stochastic-volatility model.

    Supports Monte Carlo (default) and analytic characteristic-function methods.
    """
    _check_core()
    opt_type = _pricing.OptionType.CALL if params.option_type == "call" else _pricing.OptionType.PUT

    result = _pricing.price_heston(
        underlying_price=params.spot_price,
        strike_price=params.strike_price,
        time_to_expiration=params.time_to_expiration,
        risk_free_rate=params.risk_free_rate,
        volatility=params.volatility,
        v0=params.v0,
        kappa=params.kappa,
        theta=params.theta,
        sigma=params.sigma,
        rho=params.rho,
        option_type=opt_type,
        use_monte_carlo=params.use_monte_carlo,
        num_simulations=params.num_simulations,
        num_time_steps=params.num_time_steps,
    )

    return HestonPriceResponse(
        option_type=params.option_type,
        spot_price=params.spot_price,
        strike_price=params.strike_price,
        time_to_expiration=params.time_to_expiration,
        risk_free_rate=params.risk_free_rate,
        volatility=params.volatility,
        v0=params.v0,
        kappa=params.kappa,
        theta=params.theta,
        sigma=params.sigma,
        rho=params.rho,
        method="monte_carlo" if params.use_monte_carlo else "analytic",
        price=_nan_to_none(result.price),
    )


@app.post("/greeks", response_model=GreeksResponse, tags=["greeks"])
def greeks(params: OptionParamsBase) -> GreeksResponse:
    """
    Compute Black-Scholes Greeks (delta, gamma, vega, theta, rho).

    Pass either raw parameters or a `ticker` to fetch live spot/rate.
    """
    _check_core()

    result = _pricing.greeks(
        underlying_price=params.spot_price,
        strike_price=params.strike_price,
        time_to_expiration=params.time_to_expiration,
        risk_free_rate=params.risk_free_rate,
        volatility=params.volatility,
    )

    return GreeksResponse(
        option_type=params.option_type,
        spot_price=params.spot_price,
        strike_price=params.strike_price,
        time_to_expiration=params.time_to_expiration,
        risk_free_rate=params.risk_free_rate,
        volatility=params.volatility,
        delta=_nan_to_none(result.delta),
        gamma=_nan_to_none(result.gamma),
        vega=_nan_to_none(result.vega),
        theta=_nan_to_none(result.theta),
        rho=_nan_to_none(result.rho),
    )


# ---------------------------------------------------------------------------
# Dashboard
# ---------------------------------------------------------------------------

@app.get("/", include_in_schema=False)
def dashboard():
    """Serve the HTML dashboard."""
    html_path = _ROOT / "dashboard.html"
    if not html_path.exists():
        raise HTTPException(status_code=404, detail="dashboard.html not found")
    return FileResponse(str(html_path))


# ---------------------------------------------------------------------------
# Options chain
# ---------------------------------------------------------------------------

class ChainRow(BaseModel):
    strike: float
    bid: Optional[float]
    ask: Optional[float]
    last: Optional[float]
    volume: Optional[int]
    open_interest: Optional[int]
    iv_calculated: Optional[float]
    time_to_expiry: Optional[float]
    expiry: Optional[str]
    in_the_money: Optional[bool]


@app.get("/chain/{ticker}", tags=["market-data"])
def options_chain(
    ticker: str,
    option_type: Literal["call", "put"] = Query("call"),
    expiry: Optional[str] = Query(None, description="YYYY-MM-DD expiry. Omit for nearest."),
) -> list[dict]:
    """
    Fetch live options chain for a ticker.
    Returns strikes with bid/ask, volume, open interest, and back-solved IV.
    """
    import pandas as pd

    try:
        df: pd.DataFrame = _md.get_options_chain(
            ticker,
            expiry=expiry,
            option_type=option_type,
        )
    except Exception as exc:
        raise HTTPException(status_code=502, detail=f"Market data error: {exc}") from exc

    if df.empty:
        return []

    rows = []
    for _, r in df.iterrows():
        def _fv(v):
            try:
                return None if (v is None or (isinstance(v, float) and math.isnan(v))) else float(v)
            except Exception:
                return None

        def _iv(v):
            try:
                f = float(v)
                return None if math.isnan(f) else f
            except Exception:
                return None

        rows.append({
            "strike":        _fv(r.get("strike")),
            "bid":           _fv(r.get("bid")),
            "ask":           _fv(r.get("ask")),
            "last":          _fv(r.get("lastPrice")),
            "volume":        int(r["volume"])        if r.get("volume")        not in (None, "") and not (isinstance(r.get("volume"),  float) and math.isnan(r["volume"]))  else None,
            "open_interest": int(r["openInterest"]) if r.get("openInterest") not in (None, "") and not (isinstance(r.get("openInterest"), float) and math.isnan(r["openInterest"])) else None,
            "iv_calculated": _iv(r.get("iv_calculated")),
            "time_to_expiry":_fv(r.get("time_to_expiry")),
            "expiry":        str(r["expiry"]) if r.get("expiry") is not None else None,
            "in_the_money":  bool(r["inTheMoney"]) if r.get("inTheMoney") is not None else None,
        })
    return rows


# ---------------------------------------------------------------------------
# Dev entry point
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    import uvicorn
    uvicorn.run("api:app", host="0.0.0.0", port=8000, reload=True)
