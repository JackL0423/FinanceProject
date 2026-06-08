"""
test_market_data.py — Unit tests for the market data and IV solver.

Most tests are pure-Python and don't require network access.
Tests marked @pytest.mark.live hit yFinance and require internet.
"""

import math
import pytest
from market_data import get_implied_volatility, _bs_price


# ---------------------------------------------------------------------------
# IV solver — round-trip tests (no network needed)
# ---------------------------------------------------------------------------

class TestImpliedVolatility:

    @pytest.mark.parametrize("sigma", [0.10, 0.20, 0.35, 0.60, 0.80])
    def test_iv_roundtrip_call(self, sigma):
        """BS price → IV solver → should recover original sigma."""
        S, K, T, r = 100.0, 100.0, 1.0, 0.05
        price = _bs_price(S, K, T, r, sigma, "call")
        recovered = get_implied_volatility(S, K, T, r, price, "call")
        assert abs(recovered - sigma) < 1e-4, f"sigma={sigma}  recovered={recovered}"

    @pytest.mark.parametrize("sigma", [0.10, 0.20, 0.35, 0.60])
    def test_iv_roundtrip_put(self, sigma):
        S, K, T, r = 100.0, 105.0, 0.5, 0.03
        price = _bs_price(S, K, T, r, sigma, "put")
        recovered = get_implied_volatility(S, K, T, r, price, "put")
        assert abs(recovered - sigma) < 1e-4

    def test_iv_itm_call(self):
        S, K, T, r, sigma = 110.0, 100.0, 1.0, 0.05, 0.25
        price = _bs_price(S, K, T, r, sigma, "call")
        recovered = get_implied_volatility(S, K, T, r, price, "call")
        assert abs(recovered - sigma) < 1e-4

    def test_iv_otm_put(self):
        S, K, T, r, sigma = 110.0, 100.0, 0.5, 0.03, 0.30
        price = _bs_price(S, K, T, r, sigma, "put")
        recovered = get_implied_volatility(S, K, T, r, price, "put")
        assert abs(recovered - sigma) < 1e-4

    def test_iv_returns_nan_for_zero_price(self):
        iv = get_implied_volatility(100, 100, 1.0, 0.05, 0.0, "call")
        assert math.isnan(iv)

    def test_iv_returns_nan_below_intrinsic(self):
        """Price below intrinsic value has no valid IV."""
        intrinsic = 100.0 - 90.0 * math.exp(-0.05 * 1.0)
        iv = get_implied_volatility(100.0, 90.0, 1.0, 0.05, intrinsic * 0.5, "call")
        assert math.isnan(iv)

    def test_iv_returns_nan_for_zero_tte(self):
        iv = get_implied_volatility(100, 100, 0.0, 0.05, 5.0, "call")
        assert math.isnan(iv)

    def test_iv_high_vol_recovers(self):
        """Should solve for high-vol options (σ=1.5 is outside BS validity,
        but σ=0.90 near our solver boundary should still work)."""
        S, K, T, r, sigma = 100.0, 100.0, 1.0, 0.05, 0.90
        price = _bs_price(S, K, T, r, sigma, "call")
        recovered = get_implied_volatility(S, K, T, r, price, "call")
        assert abs(recovered - sigma) < 1e-3


# ---------------------------------------------------------------------------
# Black-Scholes internal helper
# ---------------------------------------------------------------------------

class TestBSPrice:

    def test_put_call_parity(self):
        S, K, T, r, sigma = 100.0, 100.0, 1.0, 0.05, 0.20
        call = _bs_price(S, K, T, r, sigma, "call")
        put  = _bs_price(S, K, T, r, sigma, "put")
        parity = S - K * math.exp(-r * T)
        assert abs((call - put) - parity) < 1e-10

    def test_zero_vol_returns_nan(self):
        assert math.isnan(_bs_price(100, 100, 1.0, 0.05, 0.0, "call"))

    def test_zero_tte_returns_nan(self):
        assert math.isnan(_bs_price(100, 100, 0.0, 0.05, 0.2, "call"))

    def test_deep_itm_call(self):
        price = _bs_price(200, 100, 1.0, 0.05, 0.20, "call")
        intrinsic = 200 - 100 * math.exp(-0.05)
        assert abs(price - intrinsic) < 0.5

    def test_deep_otm_call_near_zero(self):
        price = _bs_price(50, 200, 0.25, 0.05, 0.20, "call")
        assert price < 0.01


# ---------------------------------------------------------------------------
# Live market data (network required)
# ---------------------------------------------------------------------------

@pytest.mark.live
def test_spot_price_positive():
    from market_data import get_spot_price
    price = get_spot_price("SPY")
    assert price > 0

@pytest.mark.live
def test_risk_free_rate_reasonable():
    from market_data import get_risk_free_rate
    rfr = get_risk_free_rate()
    assert 0 < rfr < 0.20, f"Unexpected risk-free rate: {rfr}"

@pytest.mark.live
def test_options_chain_returns_dataframe():
    from market_data import get_options_chain
    import pandas as pd
    df = get_options_chain("SPY", option_type="call")
    assert isinstance(df, pd.DataFrame)
    assert not df.empty
    assert "strike" in df.columns
    assert "iv_calculated" in df.columns

@pytest.mark.live
def test_options_chain_iv_mostly_finite():
    from market_data import get_options_chain
    df = get_options_chain("SPY", option_type="call")
    finite_pct = df["iv_calculated"].apply(lambda x: not math.isnan(x)).mean()
    assert finite_pct > 0.50, f"Only {finite_pct:.0%} of IVs are finite"

@pytest.mark.live
def test_market_snapshot_fields():
    from market_data import get_market_snapshot
    snap = get_market_snapshot("SPY")
    assert snap.spot > 0
    assert 0 < snap.risk_free_rate < 0.20
    assert snap.ticker == "SPY"
