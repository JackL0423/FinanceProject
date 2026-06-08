"""
conftest.py — shared fixtures and marks for the test suite.

Run all tests:
    conda activate finance
    export PYTHONPATH="$(pwd)/build:$PYTHONPATH"
    pytest tests/ -v

Skip live-data tests (no network):
    pytest tests/ -v -m "not live"

Skip slow Monte Carlo tests:
    pytest tests/ -v -m "not slow"
"""

import math
import sys
import os

import pytest

# Make sure the project root is on the path so imports resolve
ROOT = os.path.dirname(os.path.dirname(__file__))
sys.path.insert(0, ROOT)

# ---------------------------------------------------------------------------
# Custom marks (register to suppress PytestUnknownMarkWarning)
# ---------------------------------------------------------------------------
def pytest_configure(config):
    config.addinivalue_line("markers", "live: requires live market data (network)")
    config.addinivalue_line("markers", "slow: slow Monte Carlo simulations")
    config.addinivalue_line("markers", "quantlib: requires QuantLib to be installed")


# ---------------------------------------------------------------------------
# Shared option parameter fixtures
# ---------------------------------------------------------------------------

@pytest.fixture
def atm_call_params():
    """Standard ATM call — S=K=100, T=1yr, r=5%, σ=20%."""
    return dict(
        underlying_price=100.0,
        strike_price=100.0,
        time_to_expiration=1.0,
        risk_free_rate=0.05,
        volatility=0.20,
    )


@pytest.fixture
def itm_put_params():
    """In-the-money put — S=90, K=100, T=0.5yr, r=3%, σ=25%."""
    return dict(
        underlying_price=90.0,
        strike_price=100.0,
        time_to_expiration=0.5,
        risk_free_rate=0.03,
        volatility=0.25,
    )


@pytest.fixture
def heston_params():
    """
    Heston parameters matching expectedValues.py (QuantLib reference).
    v0=theta so the variance starts at its long-run mean.
    """
    return dict(
        underlying_price=100.0,
        strike_price=100.0,
        time_to_expiration=1.0,
        risk_free_rate=0.05,
        volatility=0.20,
        v0=0.04,
        kappa=2.0,
        theta=0.04,
        sigma=0.3,
        rho=-0.7,
    )
