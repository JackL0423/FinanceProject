"""
test_bridge.py — Unit tests for the Python → C++ pricing bridge.

Validates correctness of Black-Scholes and Heston pricing and Greeks
against known analytical results.
"""

import math
import pytest
import pricing as p

# Skip the entire module if the C++ module isn't built yet
pytestmark = pytest.mark.skipif(
    not p._CORE_AVAILABLE,
    reason="pricing_core C++ module not built. Run ./build_bridge.sh first.",
)


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _ncdf(x: float) -> float:
    return (1 + math.erf(x / math.sqrt(2))) / 2


def _bs_price(S=None, K=None, T=None, r=None, sigma=None, opt="call",
              underlying_price=None, strike_price=None,
              time_to_expiration=None, risk_free_rate=None, volatility=None) -> float:
    """Accept both positional (S,K,T,r,sigma) and fixture keyword-arg naming."""
    S = S if S is not None else underlying_price
    K = K if K is not None else strike_price
    T = T if T is not None else time_to_expiration
    r = r if r is not None else risk_free_rate
    sigma = sigma if sigma is not None else volatility
    d1 = (math.log(S / K) + (r + 0.5 * sigma ** 2) * T) / (sigma * math.sqrt(T))
    d2 = d1 - sigma * math.sqrt(T)
    if opt == "call":
        return S * _ncdf(d1) - K * math.exp(-r * T) * _ncdf(d2)
    return K * math.exp(-r * T) * _ncdf(-d2) - S * _ncdf(-d1)


# ---------------------------------------------------------------------------
# Black-Scholes — price
# ---------------------------------------------------------------------------

class TestBlackScholesPrice:

    def test_atm_call_matches_analytical(self, atm_call_params):
        result = p.price_black_scholes(**atm_call_params, option_type=p.OptionType.CALL)
        expected = _bs_price(**atm_call_params)
        assert math.isfinite(result.price)
        assert abs(result.price - expected) < 1e-4

    def test_atm_put_matches_analytical(self, atm_call_params):
        result = p.price_black_scholes(**atm_call_params, option_type=p.OptionType.PUT)
        expected = _bs_price(**atm_call_params, opt="put")  # noqa: E501
        assert math.isfinite(result.price)
        assert abs(result.price - expected) < 1e-4

    def test_put_call_parity(self, atm_call_params):
        """C - P = S - K·e^(-rT)"""
        S = atm_call_params["underlying_price"]
        K = atm_call_params["strike_price"]
        r = atm_call_params["risk_free_rate"]
        T = atm_call_params["time_to_expiration"]
        call = p.price_black_scholes(**atm_call_params, option_type=p.OptionType.CALL).price
        put  = p.price_black_scholes(**atm_call_params, option_type=p.OptionType.PUT).price
        parity = S - K * math.exp(-r * T)
        assert abs((call - put) - parity) < 1e-5

    def test_itm_put_higher_than_atm_put(self, atm_call_params, itm_put_params):
        atm_put = p.price_black_scholes(**atm_call_params, option_type=p.OptionType.PUT).price
        itm_put = p.price_black_scholes(**itm_put_params,  option_type=p.OptionType.PUT).price
        assert itm_put > atm_put

    def test_d1_d2_values(self, atm_call_params):
        """d1 and d2 should match the analytical formula."""
        S, K = atm_call_params["underlying_price"], atm_call_params["strike_price"]
        T, r  = atm_call_params["time_to_expiration"], atm_call_params["risk_free_rate"]
        sigma = atm_call_params["volatility"]
        expected_d1 = (math.log(S/K) + (r + 0.5*sigma**2)*T) / (sigma*math.sqrt(T))
        expected_d2 = expected_d1 - sigma * math.sqrt(T)
        result = p.price_black_scholes(**atm_call_params, option_type=p.OptionType.CALL)
        assert abs(result.d1 - expected_d1) < 1e-8
        assert abs(result.d2 - expected_d2) < 1e-8

    def test_deep_itm_call_approaches_intrinsic(self):
        """Deep ITM call price ≈ S - K·e^(-rT)."""
        result = p.price_black_scholes(200.0, 100.0, 1.0, 0.05, 0.20, p.OptionType.CALL)
        intrinsic = 200.0 - 100.0 * math.exp(-0.05 * 1.0)
        assert abs(result.price - intrinsic) < 0.5

    def test_deep_otm_call_near_zero(self):
        result = p.price_black_scholes(50.0, 200.0, 0.25, 0.05, 0.20, p.OptionType.CALL)
        assert result.price < 0.01


# ---------------------------------------------------------------------------
# Greeks
# ---------------------------------------------------------------------------

class TestGreeks:

    def test_call_delta_between_0_and_1(self, atm_call_params):
        g = p.greeks(**atm_call_params)
        assert 0 < g.delta < 1

    def test_atm_call_delta_near_half(self, atm_call_params):
        """ATM call delta is close to 0.5 (slightly above due to drift)."""
        g = p.greeks(**atm_call_params)
        assert 0.5 < g.delta < 0.65

    def test_gamma_positive(self, atm_call_params):
        g = p.greeks(**atm_call_params)
        assert g.gamma > 0

    def test_vega_positive(self, atm_call_params):
        g = p.greeks(**atm_call_params)
        assert g.vega > 0

    def test_theta_negative(self, atm_call_params):
        """Theta is negative — long options lose value with time."""
        g = p.greeks(**atm_call_params)
        assert g.theta < 0

    def test_rho_positive_for_call(self, atm_call_params):
        """Call rho is positive — calls gain value when rates rise."""
        g = p.greeks(**atm_call_params)
        assert g.rho > 0

    def test_delta_increases_with_spot(self):
        """Delta should be monotonically increasing with spot price."""
        deltas = [p.greeks(s, 100.0, 1.0, 0.05, 0.20).delta for s in [80, 90, 100, 110, 120]]
        assert all(deltas[i] < deltas[i+1] for i in range(len(deltas)-1))

    def test_gamma_peaks_atm(self):
        """Gamma is highest when at-the-money."""
        gamma_otm  = p.greeks(80.0,  100.0, 1.0, 0.05, 0.20).gamma
        gamma_atm  = p.greeks(100.0, 100.0, 1.0, 0.05, 0.20).gamma
        gamma_itm  = p.greeks(120.0, 100.0, 1.0, 0.05, 0.20).gamma
        assert gamma_atm > gamma_otm
        assert gamma_atm > gamma_itm

    def test_vega_peaks_atm(self):
        """Vega is highest when at-the-money."""
        vega_otm = p.greeks(70.0,  100.0, 1.0, 0.05, 0.20).vega
        vega_atm = p.greeks(100.0, 100.0, 1.0, 0.05, 0.20).vega
        vega_itm = p.greeks(130.0, 100.0, 1.0, 0.05, 0.20).vega
        assert vega_atm > vega_otm
        assert vega_atm > vega_itm


# ---------------------------------------------------------------------------
# Heston — Monte Carlo
# ---------------------------------------------------------------------------

class TestHestonMonteCarlo:

    @pytest.mark.slow
    def test_heston_call_positive(self, heston_params):
        result = p.price_heston(**heston_params, option_type=p.OptionType.CALL,
                                use_monte_carlo=True, num_simulations=20_000)
        assert math.isfinite(result.price)
        assert result.price > 0

    @pytest.mark.slow
    def test_heston_put_call_parity(self, heston_params):
        """Heston MC should satisfy put-call parity within MC noise tolerance."""
        S = heston_params["underlying_price"]
        K = heston_params["strike_price"]
        r = heston_params["risk_free_rate"]
        T = heston_params["time_to_expiration"]
        call = p.price_heston(**heston_params, option_type=p.OptionType.CALL,
                              use_monte_carlo=True, num_simulations=50_000).price
        put  = p.price_heston(**heston_params, option_type=p.OptionType.PUT,
                              use_monte_carlo=True, num_simulations=50_000).price
        parity = S - K * math.exp(-r * T)
        # MC noise tolerance is wider than analytical
        assert abs((call - put) - parity) < 1.0

    @pytest.mark.slow
    def test_heston_converges_to_bs_when_vol_constant(self):
        """
        When kappa is very high and v0=theta, Heston ≈ Black-Scholes
        because variance is pinned to theta ≈ sigma^2.
        """
        S, K, T, r, sigma = 100.0, 100.0, 1.0, 0.05, 0.20
        v0 = sigma ** 2   # 0.04

        heston_price = p.price_heston(
            S, K, T, r, sigma,
            v0=v0, kappa=50.0, theta=v0, sigma=0.001, rho=0.0,
            option_type=p.OptionType.CALL,
            use_monte_carlo=True, num_simulations=50_000, num_time_steps=200,
        ).price
        bs_price = p.price_black_scholes(S, K, T, r, sigma, p.OptionType.CALL).price

        assert abs(heston_price - bs_price) < 1.0


# ---------------------------------------------------------------------------
# QuantLib comparison (skipped if not installed)
# ---------------------------------------------------------------------------

@pytest.mark.quantlib
@pytest.mark.slow
def test_heston_matches_quantlib(heston_params):
    """Compare our Heston MC price to QuantLib's analytic Heston engine."""
    ql = pytest.importorskip("QuantLib")

    S = heston_params["underlying_price"]
    K = heston_params["strike_price"]
    T = heston_params["time_to_expiration"]
    r = heston_params["risk_free_rate"]
    v0    = heston_params["v0"]
    kappa = heston_params["kappa"]
    theta = heston_params["theta"]
    sigma = heston_params["sigma"]
    rho   = heston_params["rho"]

    # QuantLib reference price
    spot_h  = ql.QuoteHandle(ql.SimpleQuote(S))
    rate_h  = ql.YieldTermStructureHandle(
        ql.FlatForward(0, ql.NullCalendar(),
                       ql.QuoteHandle(ql.SimpleQuote(r)), ql.Actual365Fixed()))
    process = ql.HestonProcess(rate_h, rate_h, spot_h, v0, kappa, theta, sigma, rho)
    model   = ql.HestonModel(process)
    engine  = ql.AnalyticHestonEngine(model)
    payoff  = ql.PlainVanillaPayoff(ql.Option.Call, K)
    exercise = ql.EuropeanExercise(ql.Date().todaysDate() + int(T * 365))
    option  = ql.VanillaOption(payoff, exercise)
    option.setPricingEngine(engine)
    ql_price = option.NPV()

    our_price = p.price_heston(
        **heston_params, option_type=p.OptionType.CALL,
        use_monte_carlo=True, num_simulations=100_000, num_time_steps=200,
    ).price

    # MC tolerance: within 2% of QuantLib analytic price
    assert abs(our_price - ql_price) / ql_price < 0.02, (
        f"Heston MC={our_price:.4f}  QuantLib={ql_price:.4f}  "
        f"diff={abs(our_price-ql_price):.4f}"
    )
