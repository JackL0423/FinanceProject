"""
test_integration.py — End-to-end integration tests.

Chains together: market data → C++ pricing → API → IV round-trip.
All tests here are marked @pytest.mark.live (require network).
"""

import math
import pytest
from fastapi.testclient import TestClient

import pricing as p
import market_data as md

pytestmark = [
    pytest.mark.live,
    pytest.mark.skipif(
        not p._CORE_AVAILABLE,
        reason="pricing_core not built. Run ./build_bridge.sh first.",
    ),
]

from api import app
client = TestClient(app)


# ---------------------------------------------------------------------------
# Live data → pricing bridge
# ---------------------------------------------------------------------------

def test_live_spot_into_bs_bridge():
    """Fetch SPY spot live, then price an ATM call via the C++ bridge."""
    snap = md.get_market_snapshot("SPY")
    result = p.price_black_scholes(
        underlying_price=snap.spot,
        strike_price=round(snap.spot / 5) * 5,   # nearest $5 strike
        time_to_expiration=0.25,
        risk_free_rate=snap.risk_free_rate,
        volatility=0.15,
        option_type=p.OptionType.CALL,
    )
    assert math.isfinite(result.price)
    assert result.price > 0


def test_live_data_put_call_parity():
    """End-to-end put-call parity using live spot and rate."""
    snap = md.get_market_snapshot("SPY")
    S = snap.spot
    K = round(S / 5) * 5
    T, r, sigma = 0.5, snap.risk_free_rate, 0.15

    call = p.price_black_scholes(S, K, T, r, sigma, p.OptionType.CALL).price
    put  = p.price_black_scholes(S, K, T, r, sigma, p.OptionType.PUT).price
    parity = S - K * math.exp(-r * T)
    assert abs((call - put) - parity) < 1e-4


# ---------------------------------------------------------------------------
# Live data → API
# ---------------------------------------------------------------------------

def test_api_ticker_param_resolves_spot():
    """Pass a ticker to the API; it should resolve spot & rate automatically."""
    r = client.post("/price/black-scholes", json={
        "ticker": "SPY",
        "strike_price": 700.0,
        "time_to_expiration": 0.25,
        "volatility": 0.15,
        "option_type": "call",
    })
    assert r.status_code == 200
    body = r.json()
    assert body["spot_price"] > 0
    assert body["risk_free_rate"] > 0
    assert math.isfinite(body["price"])


def test_api_greeks_with_live_spot():
    r = client.post("/greeks", json={
        "ticker": "SPY",
        "strike_price": 700.0,
        "time_to_expiration": 0.25,
        "volatility": 0.15,
    })
    assert r.status_code == 200
    body = r.json()
    assert 0 < body["delta"] < 1
    assert body["gamma"] > 0


# ---------------------------------------------------------------------------
# Live options chain → IV round-trip
# ---------------------------------------------------------------------------

def test_options_chain_iv_roundtrip():
    """
    Fetch live call chain, back-solve IV from market mid-price,
    then re-price with that IV and verify we recover the mid within $0.10.
    """
    snap = md.get_market_snapshot("SPY")
    df   = md.get_options_chain("SPY", option_type="call")

    # Use the 5 most liquid ATM contracts
    df = df.dropna(subset=["iv_calculated"])
    df = df[df["iv_calculated"] > 0]
    df["moneyness"] = (df["strike"] - snap.spot).abs()
    top5 = df.nsmallest(5, "moneyness")

    for _, row in top5.iterrows():
        if math.isnan(row["iv_calculated"]):
            continue
        repriced = md._bs_price(
            snap.spot,
            row["strike"],
            row["time_to_expiry"],
            snap.risk_free_rate,
            row["iv_calculated"],
            "call",
        )
        mid = (row["bid"] + row["ask"]) / 2
        assert abs(repriced - mid) < 0.10, (
            f"IV round-trip failed: strike={row['strike']} "
            f"mid={mid:.4f} repriced={repriced:.4f} IV={row['iv_calculated']:.4f}"
        )


# ---------------------------------------------------------------------------
# Full pipeline smoke test
# ---------------------------------------------------------------------------

@pytest.mark.slow
def test_full_pipeline_spy():
    """
    Full pipeline:
      1. Fetch live SPY spot + rate
      2. Get ATM call from options chain
      3. Price it with BS bridge
      4. Compute Greeks
      5. Verify BS price is within 20% of market mid (model vs market spread)
    """
    snap  = md.get_market_snapshot("SPY")
    chain = md.get_options_chain("SPY", option_type="call")

    chain = chain[chain["bid"] > 0]
    chain["moneyness"] = (chain["strike"] - snap.spot).abs()
    atm = chain.nsmallest(1, "moneyness").iloc[0]

    strike = atm["strike"]
    tte    = atm["time_to_expiry"]
    mid    = (atm["bid"] + atm["ask"]) / 2
    iv     = atm["iv_calculated"] if not math.isnan(atm["iv_calculated"]) else 0.15

    # Price
    bs_result = p.price_black_scholes(
        snap.spot, strike, tte, snap.risk_free_rate, iv, p.OptionType.CALL
    )

    # Greeks
    greeks = p.greeks(snap.spot, strike, tte, snap.risk_free_rate, iv)

    # Assertions
    assert math.isfinite(bs_result.price)
    assert 0 < greeks.delta < 1
    assert greeks.gamma > 0
    assert greeks.vega > 0

    # BS price (using market IV) should be very close to mid since we back-solved IV from mid
    assert abs(bs_result.price - mid) < 0.10, (
        f"BS price={bs_result.price:.4f} vs market mid={mid:.4f}"
    )
