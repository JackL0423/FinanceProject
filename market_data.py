"""
market_data.py — Fetch live market data and compute implied volatility.

Requires:
    pip install yfinance scipy pandas

Optional (real-time quotes):
    pip install alpaca-trade-api
    Set env vars: ALPACA_API_KEY, ALPACA_SECRET_KEY

Usage:
    from market_data import get_spot_price, get_options_chain, get_implied_volatility
"""

from __future__ import annotations

import math
import os
import warnings
from dataclasses import dataclass, field
from datetime import date, datetime, timedelta, timezone
from typing import Optional

import pandas as pd

try:
    import yfinance as yf
    _YF_AVAILABLE = True
except ImportError:
    _YF_AVAILABLE = False

try:
    import scipy.optimize as opt
    _SCIPY_AVAILABLE = True
except ImportError:
    _SCIPY_AVAILABLE = False


# ---------------------------------------------------------------------------
# Result types
# ---------------------------------------------------------------------------

@dataclass
class OptionRow:
    """A single option contract from the chain."""
    ticker: str
    expiry: date
    strike: float
    option_type: str          # "call" or "put"
    last_price: float
    bid: float
    ask: float
    implied_vol: float        # from exchange (may be NaN)
    iv_calculated: float      # back-solved from BS (NaN if unsolvable)
    time_to_expiry: float     # in years
    open_interest: int
    volume: int


@dataclass
class MarketSnapshot:
    """Spot price + risk-free rate for a ticker at a point in time."""
    ticker: str
    spot: float
    risk_free_rate: float     # annualised, continuously compounded
    timestamp: datetime = field(default_factory=lambda: datetime.now(timezone.utc))


# ---------------------------------------------------------------------------
# Spot price
# ---------------------------------------------------------------------------

def get_spot_price(ticker: str, use_alpaca: bool = False) -> float:
    """
    Return the current spot price for *ticker*.

    Tries Alpaca first if use_alpaca=True and credentials are set,
    otherwise falls back to yFinance.
    """
    if use_alpaca:
        price = _alpaca_spot(ticker)
        if price is not None:
            return price

    return _yf_spot(ticker)


def _yf_spot(ticker: str) -> float:
    if not _YF_AVAILABLE:
        raise ImportError("yfinance is required: pip install yfinance")
    t = yf.Ticker(ticker)
    info = t.fast_info
    price = getattr(info, "last_price", None) or getattr(info, "regularMarketPrice", None)
    if price is None:
        # fallback: last close from 2-day history
        hist = t.history(period="2d")
        if hist.empty:
            raise ValueError(f"No price data found for {ticker!r}")
        price = float(hist["Close"].iloc[-1])
    return float(price)


def _alpaca_spot(ticker: str) -> Optional[float]:
    """Return latest trade price from Alpaca, or None if unavailable."""
    try:
        import alpaca_trade_api as tradeapi  # type: ignore
        key    = os.environ.get("ALPACA_API_KEY", "")
        secret = os.environ.get("ALPACA_SECRET_KEY", "")
        if not key or not secret:
            return None
        api = tradeapi.REST(key, secret, base_url="https://data.alpaca.markets")
        trade = api.get_latest_trade(ticker)
        return float(trade.price)
    except Exception:
        return None


# ---------------------------------------------------------------------------
# Risk-free rate (3-month T-bill proxy via yFinance)
# ---------------------------------------------------------------------------

def get_risk_free_rate() -> float:
    """
    Return the current annualised risk-free rate (3-month US T-bill yield).
    Falls back to 0.05 (5%) if the fetch fails.
    """
    if not _YF_AVAILABLE:
        return 0.05
    try:
        tbill = yf.Ticker("^IRX")   # 13-week T-bill index (annualised %)
        hist = tbill.history(period="5d")
        if not hist.empty:
            return float(hist["Close"].iloc[-1]) / 100.0
    except Exception:
        pass
    return 0.05


# ---------------------------------------------------------------------------
# Options chain
# ---------------------------------------------------------------------------

def get_options_chain(
    ticker: str,
    expiry: Optional[str] = None,
    option_type: str = "call",
    risk_free_rate: Optional[float] = None,
) -> pd.DataFrame:
    """
    Fetch the options chain for *ticker* and return a tidy DataFrame.

    Parameters
    ----------
    ticker       : e.g. "SPY", "AAPL"
    expiry       : "YYYY-MM-DD". If None, uses the nearest available expiry.
    option_type  : "call", "put", or "both"
    risk_free_rate : override for IV calculation; fetched automatically if None

    Returns a DataFrame with columns:
        strike, last_price, bid, ask, implied_vol, iv_calculated,
        time_to_expiry, open_interest, volume, option_type
    """
    if not _YF_AVAILABLE:
        raise ImportError("yfinance is required: pip install yfinance")

    t = yf.Ticker(ticker)
    expirations = t.options
    if not expirations:
        raise ValueError(f"No options data available for {ticker!r}")

    if expiry is None:
        expiry = expirations[0]
    elif expiry not in expirations:
        # pick nearest available
        target = datetime.strptime(expiry, "%Y-%m-%d").date()
        expiry = min(expirations, key=lambda e: abs((datetime.strptime(e, "%Y-%m-%d").date() - target).days))
        warnings.warn(f"Exact expiry not available; using nearest: {expiry}")

    chain = t.option_chain(expiry)
    spot  = get_spot_price(ticker)
    rfr   = risk_free_rate if risk_free_rate is not None else get_risk_free_rate()

    expiry_date = datetime.strptime(expiry, "%Y-%m-%d").date()
    tte = max((expiry_date - date.today()).days / 365.0, 1e-6)

    frames = []
    if option_type in ("call", "both"):
        frames.append(_process_chain_df(chain.calls, ticker, expiry_date, "call", spot, rfr, tte))
    if option_type in ("put", "both"):
        frames.append(_process_chain_df(chain.puts,  ticker, expiry_date, "put",  spot, rfr, tte))

    return pd.concat(frames, ignore_index=True) if frames else pd.DataFrame()


def _process_chain_df(
    df: pd.DataFrame,
    ticker: str,
    expiry_date: date,
    opt_type: str,
    spot: float,
    rfr: float,
    tte: float,
) -> pd.DataFrame:
    rows = []
    for _, r in df.iterrows():
        strike     = float(r.get("strike", math.nan))
        last_price = float(r.get("lastPrice", math.nan))
        bid        = float(r.get("bid", math.nan))
        ask        = float(r.get("ask", math.nan))
        exch_iv    = float(r.get("impliedVolatility", math.nan))
        raw_oi  = r.get("openInterest", 0)
        raw_vol = r.get("volume", 0)
        oi  = 0 if (raw_oi  is None or (isinstance(raw_oi,  float) and math.isnan(raw_oi)))  else int(raw_oi)
        vol = 0 if (raw_vol is None or (isinstance(raw_vol, float) and math.isnan(raw_vol))) else int(raw_vol)

        mid_price = (bid + ask) / 2.0 if not (math.isnan(bid) or math.isnan(ask)) else last_price
        calc_iv = _implied_vol(spot, strike, tte, rfr, mid_price, opt_type)

        rows.append({
            "ticker":         ticker,
            "expiry":         expiry_date,
            "strike":         strike,
            "option_type":    opt_type,
            "last_price":     last_price,
            "bid":            bid,
            "ask":            ask,
            "implied_vol":    exch_iv,
            "iv_calculated":  calc_iv,
            "time_to_expiry": tte,
            "open_interest":  oi,
            "volume":         vol,
        })
    return pd.DataFrame(rows)


# ---------------------------------------------------------------------------
# Implied volatility (bisection on Black-Scholes)
# ---------------------------------------------------------------------------

def get_implied_volatility(
    spot: float,
    strike: float,
    time_to_expiry: float,
    risk_free_rate: float,
    market_price: float,
    option_type: str = "call",
) -> float:
    """
    Back-solve Black-Scholes for implied volatility given a market price.

    Returns NaN if the price is outside the no-arbitrage bounds or
    the solver fails to converge.
    """
    return _implied_vol(spot, strike, time_to_expiry, risk_free_rate, market_price, option_type)


def _bs_price(spot: float, strike: float, tte: float, rfr: float,
              sigma: float, opt_type: str) -> float:
    """Black-Scholes price (pure Python, no C++ dependency)."""
    if sigma <= 0 or tte <= 0:
        return math.nan
    d1 = (math.log(spot / strike) + (rfr + 0.5 * sigma ** 2) * tte) / (sigma * math.sqrt(tte))
    d2 = d1 - sigma * math.sqrt(tte)

    def _ncdf(x: float) -> float:
        return (1.0 + math.erf(x / math.sqrt(2.0))) / 2.0

    if opt_type == "call":
        return spot * _ncdf(d1) - strike * math.exp(-rfr * tte) * _ncdf(d2)
    else:
        return strike * math.exp(-rfr * tte) * _ncdf(-d2) - spot * _ncdf(-d1)


def _implied_vol(
    spot: float,
    strike: float,
    tte: float,
    rfr: float,
    market_price: float,
    opt_type: str,
) -> float:
    """Bisection solver for implied volatility. Returns NaN on failure."""
    if not _SCIPY_AVAILABLE:
        warnings.warn("scipy not available; install it for IV calculation.")
        return math.nan
    if math.isnan(market_price) or market_price <= 0 or tte <= 0:
        return math.nan

    # Intrinsic value bounds check
    discount = math.exp(-rfr * tte)
    if opt_type == "call":
        intrinsic = max(spot - strike * discount, 0.0)
        upper_bound = spot
    else:
        intrinsic = max(strike * discount - spot, 0.0)
        upper_bound = strike * discount

    if market_price < intrinsic - 1e-4 or market_price > upper_bound + 1e-4:
        return math.nan

    try:
        iv = opt.brentq(
            lambda sigma: _bs_price(spot, strike, tte, rfr, sigma, opt_type) - market_price,
            a=1e-6,
            b=10.0,
            xtol=1e-6,
            maxiter=200,
        )
        return iv
    except (ValueError, RuntimeError):
        return math.nan


# ---------------------------------------------------------------------------
# Convenience: full snapshot (spot + chain + IVs)
# ---------------------------------------------------------------------------

def get_market_snapshot(ticker: str) -> MarketSnapshot:
    """Return a MarketSnapshot with current spot price and risk-free rate."""
    return MarketSnapshot(
        ticker=ticker,
        spot=get_spot_price(ticker),
        risk_free_rate=get_risk_free_rate(),
    )


# ---------------------------------------------------------------------------
# Quick smoke-test  (python market_data.py SPY)
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    import sys

    ticker = sys.argv[1] if len(sys.argv) > 1 else "SPY"

    print(f"=== Market snapshot: {ticker} ===")
    snap = get_market_snapshot(ticker)
    print(f"  Spot price    : ${snap.spot:.2f}")
    print(f"  Risk-free rate: {snap.risk_free_rate*100:.2f}%")

    print(f"\n=== Nearest call chain for {ticker} ===")
    df = get_options_chain(ticker, option_type="call")
    # Show 5 strikes nearest to ATM
    atm = df.iloc[(df["strike"] - snap.spot).abs().argsort()[:5]].copy()
    atm = atm[["strike", "last_price", "bid", "ask", "implied_vol", "iv_calculated",
                "time_to_expiry", "open_interest"]].reset_index(drop=True)
    print(atm.to_string(index=False, float_format="{:.4f}".format))
