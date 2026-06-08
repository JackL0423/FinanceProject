"""
pricing.py — Python wrapper around the C++ pricing_core extension module.

Build the C++ module first:
    cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
    # pricing_core.so will appear in build/

Then either copy pricing_core.so here or add build/ to PYTHONPATH:
    export PYTHONPATH=$PYTHONPATH:$(pwd)/build

Usage:
    from pricing import price_black_scholes, price_heston, greeks, OptionType
"""

from __future__ import annotations

import math
from dataclasses import dataclass
from typing import Literal

try:
    import pricing_core as _core
    _CORE_AVAILABLE = True
except ImportError:
    _CORE_AVAILABLE = False
    import warnings
    warnings.warn(
        "pricing_core C++ module not found. "
        "Build it with: cmake -B build && cmake --build build",
        ImportWarning,
        stacklevel=2,
    )


# Re-export the enum so callers only need to import from this module
if _CORE_AVAILABLE:
    OptionType = _core.OptionType
else:
    from enum import Enum
    class OptionType(Enum):  # type: ignore[no-redef]
        CALL = 0
        PUT = 1


# ---------------------------------------------------------------------------
# Result dataclasses
# ---------------------------------------------------------------------------

@dataclass
class BSResult:
    """Black-Scholes pricing result."""
    price: float
    d1: float
    d2: float
    option_type: object  # OptionType


@dataclass
class Greeks:
    """Standard option Greeks."""
    delta: float
    gamma: float
    vega: float
    theta: float
    rho: float


@dataclass
class HestonResult:
    """Heston model pricing result."""
    price: float
    option_type: object  # OptionType


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def price_black_scholes(
    underlying_price: float,
    strike_price: float,
    time_to_expiration: float,
    risk_free_rate: float,
    volatility: float,
    option_type: object = None,
) -> BSResult:
    """
    Price a European option using Black-Scholes.

    Parameters
    ----------
    underlying_price    : current spot price of the underlying
    strike_price        : option strike price
    time_to_expiration  : time to expiry in years (e.g. 0.5 for 6 months)
    risk_free_rate      : continuously compounded risk-free rate (e.g. 0.05)
    volatility          : annualised implied volatility in (0, 1) (e.g. 0.2)
    option_type         : OptionType.CALL (default) or OptionType.PUT
    """
    if not _CORE_AVAILABLE:
        raise RuntimeError("pricing_core C++ module is not available.")

    if option_type is None:
        option_type = OptionType.CALL

    model = _core.BlackScholesModel(
        underlying_price,
        strike_price,
        time_to_expiration,
        risk_free_rate,
        volatility,
        option_type,
    )
    return BSResult(
        price=model.calculate_option_price(),
        d1=model.d1,
        d2=model.d2,
        option_type=option_type,
    )


def greeks(
    underlying_price: float,
    strike_price: float,
    time_to_expiration: float,
    risk_free_rate: float,
    volatility: float,
) -> Greeks:
    """
    Compute standard Black-Scholes Greeks (delta, gamma, vega, theta, rho).

    Delta is the call delta N(d1); put delta is N(d1) - 1.
    """
    if not _CORE_AVAILABLE:
        raise RuntimeError("pricing_core C++ module is not available.")

    g = _core.OptionGreeks(
        underlying_price,
        strike_price,
        time_to_expiration,
        risk_free_rate,
        volatility,
    )
    return Greeks(
        delta=g.delta,
        gamma=g.gamma,
        vega=g.vega,
        theta=g.theta,
        rho=g.rho,
    )


def price_heston(
    underlying_price: float,
    strike_price: float,
    time_to_expiration: float,
    risk_free_rate: float,
    volatility: float,
    v0: float,
    kappa: float,
    theta: float,
    sigma: float,
    rho: float,
    option_type: object = None,
    use_monte_carlo: bool = True,
    num_simulations: int = 10_000,
    num_time_steps: int = 100,
) -> HestonResult:
    """
    Price a European option using the Heston stochastic-volatility model.

    Parameters
    ----------
    underlying_price    : current spot price
    strike_price        : option strike
    time_to_expiration  : time to expiry in years
    risk_free_rate      : continuously compounded risk-free rate
    volatility          : initial volatility (used as starting σ for BS baseline)
    v0                  : initial variance (v0 = σ₀²)
    kappa               : mean-reversion speed of variance
    theta               : long-run mean variance
    sigma               : volatility of variance (vol-of-vol)
    rho                 : correlation between spot and variance Brownian motions
    option_type         : OptionType.CALL (default) or OptionType.PUT
    use_monte_carlo     : True = Monte Carlo, False = analytic characteristic fn
    num_simulations     : MC paths (ignored when use_monte_carlo=False)
    num_time_steps      : discretisation steps per path
    """
    if not _CORE_AVAILABLE:
        raise RuntimeError("pricing_core C++ module is not available.")

    if option_type is None:
        option_type = OptionType.CALL

    model = _core.HestonModel(
        underlying_price,
        strike_price,
        time_to_expiration,
        risk_free_rate,
        volatility,
        v0,
        kappa,
        theta,
        sigma,
        rho,
        option_type,
    )
    price = model.calculate_option_price(
        use_monte_carlo=use_monte_carlo,
        num_simulations=num_simulations,
        num_time_steps=num_time_steps,
    )
    return HestonResult(price=price, option_type=option_type)


# ---------------------------------------------------------------------------
# Quick smoke-test (python pricing.py)
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    if not _CORE_AVAILABLE:
        print("pricing_core not built yet — run cmake first.")
    else:
        print("=== Black-Scholes ===")
        bs = price_black_scholes(100, 100, 1.0, 0.05, 0.2, OptionType.CALL)
        print(f"  Call price : {bs.price:.4f}")
        print(f"  d1={bs.d1:.4f}  d2={bs.d2:.4f}")

        bs_put = price_black_scholes(100, 100, 1.0, 0.05, 0.2, OptionType.PUT)
        print(f"  Put  price : {bs_put.price:.4f}")
        print(f"  Put-call parity check (should be ~0): "
              f"{bs.price - bs_put.price - 100 + 100*math.exp(-0.05):.6f}")

        print("\n=== Greeks ===")
        g = greeks(100, 100, 1.0, 0.05, 0.2)
        print(f"  delta={g.delta:.4f}  gamma={g.gamma:.4f}  "
              f"vega={g.vega:.4f}  theta={g.theta:.4f}  rho={g.rho:.4f}")

        print("\n=== Heston (Monte Carlo) ===")
        h = price_heston(100, 100, 1.0, 0.05, 0.2,
                         v0=0.04, kappa=2.0, theta=0.04, sigma=0.3, rho=-0.7,
                         option_type=OptionType.CALL)
        print(f"  Call price : {h.price:.4f}")
