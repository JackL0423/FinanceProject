"""
test_api.py — FastAPI endpoint tests using the HTTPX test client.

These tests run without a live server and without network access
(spot/rate are passed explicitly so yFinance is never called).
"""

import math
import pytest
from fastapi.testclient import TestClient

import pricing as p

# Skip the whole module if the C++ core isn't built
pytestmark = pytest.mark.skipif(
    not p._CORE_AVAILABLE,
    reason="pricing_core not built. Run ./build_bridge.sh first.",
)

from api import app
client = TestClient(app)


# ---------------------------------------------------------------------------
# /health
# ---------------------------------------------------------------------------

def test_health_returns_ok():
    r = client.get("/health")
    assert r.status_code == 200
    body = r.json()
    assert body["status"] == "ok"
    assert body["pricing_core_loaded"] is True


# ---------------------------------------------------------------------------
# POST /price/black-scholes
# ---------------------------------------------------------------------------

class TestBSPriceEndpoint:

    BASE = dict(
        spot_price=100.0,
        strike_price=100.0,
        time_to_expiration=1.0,
        risk_free_rate=0.05,
        volatility=0.20,
        option_type="call",
    )

    def test_call_returns_200(self):
        r = client.post("/price/black-scholes", json=self.BASE)
        assert r.status_code == 200

    def test_call_price_is_finite(self):
        r = client.post("/price/black-scholes", json=self.BASE)
        assert math.isfinite(r.json()["price"])

    def test_call_price_value(self):
        r = client.post("/price/black-scholes", json=self.BASE)
        # Known ATM BS call ≈ 10.45
        assert abs(r.json()["price"] - 10.45) < 0.05

    def test_put_call_parity(self):
        call_r = client.post("/price/black-scholes", json={**self.BASE, "option_type": "call"})
        put_r  = client.post("/price/black-scholes", json={**self.BASE, "option_type": "put"})
        call_price = call_r.json()["price"]
        put_price  = put_r.json()["price"]
        S = self.BASE["spot_price"]
        K = self.BASE["strike_price"]
        r = self.BASE["risk_free_rate"]
        T = self.BASE["time_to_expiration"]
        parity = S - K * math.exp(-r * T)
        assert abs((call_price - put_price) - parity) < 1e-4

    def test_d1_d2_present(self):
        r = client.post("/price/black-scholes", json=self.BASE)
        body = r.json()
        assert "d1" in body and body["d1"] is not None
        assert "d2" in body and body["d2"] is not None
        assert body["d1"] > body["d2"]

    def test_missing_spot_and_ticker_returns_422(self):
        payload = {k: v for k, v in self.BASE.items() if k != "spot_price"}
        r = client.post("/price/black-scholes", json=payload)
        assert r.status_code == 422

    def test_invalid_volatility_returns_422(self):
        r = client.post("/price/black-scholes", json={**self.BASE, "volatility": 1.5})
        assert r.status_code == 422

    def test_invalid_option_type_returns_422(self):
        r = client.post("/price/black-scholes", json={**self.BASE, "option_type": "straddle"})
        assert r.status_code == 422

    def test_response_echoes_params(self):
        r = client.post("/price/black-scholes", json=self.BASE)
        body = r.json()
        assert body["spot_price"]    == self.BASE["spot_price"]
        assert body["strike_price"]  == self.BASE["strike_price"]
        assert body["volatility"]    == self.BASE["volatility"]
        assert body["model"]         == "black-scholes"


# ---------------------------------------------------------------------------
# POST /greeks
# ---------------------------------------------------------------------------

class TestGreeksEndpoint:

    BASE = dict(
        spot_price=100.0,
        strike_price=100.0,
        time_to_expiration=1.0,
        risk_free_rate=0.05,
        volatility=0.20,
        option_type="call",
    )

    def test_returns_200(self):
        r = client.post("/greeks", json=self.BASE)
        assert r.status_code == 200

    def test_all_greeks_present(self):
        body = client.post("/greeks", json=self.BASE).json()
        for greek in ("delta", "gamma", "vega", "theta", "rho"):
            assert greek in body and body[greek] is not None

    def test_call_delta_between_0_and_1(self):
        body = client.post("/greeks", json=self.BASE).json()
        assert 0 < body["delta"] < 1

    def test_gamma_positive(self):
        body = client.post("/greeks", json=self.BASE).json()
        assert body["gamma"] > 0

    def test_theta_negative(self):
        body = client.post("/greeks", json=self.BASE).json()
        assert body["theta"] < 0

    def test_vega_positive(self):
        body = client.post("/greeks", json=self.BASE).json()
        assert body["vega"] > 0

    def test_rho_positive_for_call(self):
        body = client.post("/greeks", json=self.BASE).json()
        assert body["rho"] > 0


# ---------------------------------------------------------------------------
# POST /price/heston
# ---------------------------------------------------------------------------

class TestHestonEndpoint:

    BASE = dict(
        spot_price=100.0,
        strike_price=100.0,
        time_to_expiration=1.0,
        risk_free_rate=0.05,
        volatility=0.20,
        v0=0.04,
        kappa=2.0,
        theta=0.04,
        sigma=0.3,
        rho=-0.7,
        option_type="call",
        use_monte_carlo=True,
        num_simulations=5000,
        num_time_steps=50,
    )

    @pytest.mark.slow
    def test_returns_200(self):
        r = client.post("/price/heston", json=self.BASE)
        assert r.status_code == 200

    @pytest.mark.slow
    def test_price_is_finite_and_positive(self):
        r = client.post("/price/heston", json=self.BASE)
        assert math.isfinite(r.json()["price"])
        assert r.json()["price"] > 0

    @pytest.mark.slow
    def test_method_field_is_monte_carlo(self):
        r = client.post("/price/heston", json=self.BASE)
        assert r.json()["method"] == "monte_carlo"

    def test_invalid_rho_returns_422(self):
        r = client.post("/price/heston", json={**self.BASE, "rho": 1.5})
        assert r.status_code == 422

    def test_negative_v0_returns_422(self):
        r = client.post("/price/heston", json={**self.BASE, "v0": -0.01})
        assert r.status_code == 422
