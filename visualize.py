"""
visualize.py — Visualization layer for the financial pricing core.

Three chart types:
  1. plot_payoff()       — P&L at expiry vs underlying price
  2. plot_greeks()       — Delta, gamma, vega, theta, rho vs spot price
  3. plot_vol_surface()  — 3-D implied volatility surface (strike × expiry)

Requirements:
    pip install matplotlib numpy

Usage:
    python visualize.py SPY          # all three charts for SPY
    python visualize.py              # demo with synthetic parameters

Or import individually:
    from visualize import plot_payoff, plot_greeks, plot_vol_surface
"""

from __future__ import annotations

import math
import sys
import warnings
from typing import Optional

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 (registers 3-D projection)

import pricing as _pricing
import market_data as _md

# ---------------------------------------------------------------------------
# Colour palette
# ---------------------------------------------------------------------------
_CALL_COLOR = "#4C72B0"
_PUT_COLOR  = "#DD8452"
_GRID_ALPHA = 0.25
_FIG_DPI    = 130


# ---------------------------------------------------------------------------
# 1. Payoff diagram
# ---------------------------------------------------------------------------

def plot_payoff(
    strike: float,
    premium: float,
    option_type: str = "call",
    spot_range_pct: float = 0.30,
    current_spot: Optional[float] = None,
    title: Optional[str] = None,
    ax: Optional[plt.Axes] = None,
) -> plt.Figure:
    """
    Plot the P&L profile at expiry for a single long option.

    Parameters
    ----------
    strike         : option strike price
    premium        : option price paid (cost basis)
    option_type    : "call" or "put"
    spot_range_pct : how far around the strike to plot (0.30 = ±30%)
    current_spot   : if provided, draws a vertical dashed line
    title          : override the default title
    ax             : existing Axes to draw into (creates new figure if None)
    """
    lo = strike * (1 - spot_range_pct)
    hi = strike * (1 + spot_range_pct)
    spots = np.linspace(lo, hi, 500)

    if option_type == "call":
        payoff = np.maximum(spots - strike, 0) - premium
    else:
        payoff = np.maximum(strike - spots, 0) - premium

    color = _CALL_COLOR if option_type == "call" else _PUT_COLOR

    standalone = ax is None
    if standalone:
        fig, ax = plt.subplots(figsize=(8, 4.5), dpi=_FIG_DPI)
    else:
        fig = ax.get_figure()

    ax.axhline(0, color="black", linewidth=0.8)
    ax.axvline(strike, color="grey", linewidth=0.8, linestyle="--", label=f"Strike ${strike:,.2f}")

    if current_spot is not None:
        ax.axvline(current_spot, color="crimson", linewidth=1.0,
                   linestyle=":", label=f"Spot ${current_spot:,.2f}")

    ax.plot(spots, payoff, color=color, linewidth=2,
            label=f"Long {option_type.capitalize()} P&L")
    ax.fill_between(spots, payoff, 0,
                    where=(payoff >= 0), color=color, alpha=0.15)
    ax.fill_between(spots, payoff, 0,
                    where=(payoff < 0), color="red", alpha=0.10)

    ax.set_xlabel("Underlying price at expiry")
    ax.set_ylabel("P&L per contract")
    ax.set_title(title or f"Payoff — Long {option_type.capitalize()} | K=${strike:,.0f} | Premium=${premium:.2f}")
    ax.xaxis.set_major_formatter(mticker.FormatStrFormatter("$%.0f"))
    ax.yaxis.set_major_formatter(mticker.FormatStrFormatter("$%.2f"))
    ax.legend(fontsize=9)
    ax.grid(True, alpha=_GRID_ALPHA)
    ax.spines[["top", "right"]].set_visible(False)

    if standalone:
        fig.tight_layout()
    return fig


# ---------------------------------------------------------------------------
# 2. Greeks vs spot price
# ---------------------------------------------------------------------------

def plot_greeks(
    strike: float,
    time_to_expiration: float,
    risk_free_rate: float,
    volatility: float,
    spot_range_pct: float = 0.30,
    current_spot: Optional[float] = None,
    title_prefix: str = "",
) -> plt.Figure:
    """
    Plot all five Greeks as a function of underlying spot price.

    Each Greek gets its own subplot so scales don't interfere.
    """
    if not _pricing._CORE_AVAILABLE:
        raise RuntimeError("pricing_core C++ module not available.")

    lo = strike * (1 - spot_range_pct)
    hi = strike * (1 + spot_range_pct)
    spots = np.linspace(lo, hi, 300)

    deltas, gammas, vegas, thetas, rhos = [], [], [], [], []
    for s in spots:
        try:
            g = _pricing.greeks(s, strike, time_to_expiration, risk_free_rate, volatility)
            deltas.append(g.delta)
            gammas.append(g.gamma)
            vegas.append(g.vega)
            thetas.append(g.theta)
            rhos.append(g.rho)
        except Exception:
            for lst in (deltas, gammas, vegas, thetas, rhos):
                lst.append(math.nan)

    greek_data = [
        ("Delta",  deltas,  _CALL_COLOR),
        ("Gamma",  gammas,  "#2CA02C"),
        ("Vega",   vegas,   "#9467BD"),
        ("Theta",  thetas,  _PUT_COLOR),
        ("Rho",    rhos,    "#8C564B"),
    ]

    fig, axes = plt.subplots(2, 3, figsize=(14, 7), dpi=_FIG_DPI)
    axes_flat = axes.flatten()

    for idx, (name, values, color) in enumerate(greek_data):
        ax = axes_flat[idx]
        ax.plot(spots, values, color=color, linewidth=1.8)
        ax.axvline(strike, color="grey", linewidth=0.8, linestyle="--", label=f"Strike ${strike:,.0f}")
        if current_spot is not None:
            ax.axvline(current_spot, color="crimson", linewidth=1.0,
                       linestyle=":", label=f"Spot ${current_spot:,.0f}")
        ax.set_title(name, fontsize=11, fontweight="bold")
        ax.set_xlabel("Spot price")
        ax.xaxis.set_major_formatter(mticker.FormatStrFormatter("$%.0f"))
        ax.grid(True, alpha=_GRID_ALPHA)
        ax.spines[["top", "right"]].set_visible(False)
        if idx == 0:
            ax.legend(fontsize=8)

    # hide the unused 6th subplot
    axes_flat[-1].set_visible(False)

    prefix = f"{title_prefix} — " if title_prefix else ""
    fig.suptitle(
        f"{prefix}Greeks vs Spot  |  K=${strike:,.0f}  σ={volatility:.0%}  T={time_to_expiration:.2f}y",
        fontsize=12, y=1.01,
    )
    fig.tight_layout()
    return fig


# ---------------------------------------------------------------------------
# 3. Implied volatility surface
# ---------------------------------------------------------------------------

def plot_vol_surface(
    ticker: str,
    max_expiries: int = 6,
    moneyness_range: float = 0.20,
    option_type: str = "call",
) -> plt.Figure:
    """
    Fetch live options data and plot the implied volatility surface.

    X axis : strike price (filtered to ±moneyness_range of spot)
    Y axis : time to expiry (years)
    Z axis : calculated implied volatility
    """
    import yfinance as yf

    t = yf.Ticker(ticker)
    expirations = t.options
    if not expirations:
        raise ValueError(f"No options data for {ticker!r}")

    snap = _md.get_market_snapshot(ticker)
    spot = snap.spot
    rfr  = snap.risk_free_rate

    expirations = expirations[:max_expiries]

    all_strikes, all_ttes, all_ivs = [], [], []

    for expiry_str in expirations:
        try:
            df = _md.get_options_chain(
                ticker,
                expiry=expiry_str,
                option_type=option_type,
                risk_free_rate=rfr,
            )
        except Exception as exc:
            warnings.warn(f"Skipping {expiry_str}: {exc}")
            continue

        df = df[df["iv_calculated"].notna() & (df["iv_calculated"] > 0)]
        # filter to strikes within moneyness_range of spot
        df = df[df["strike"].between(spot * (1 - moneyness_range),
                                      spot * (1 + moneyness_range))]
        if df.empty:
            continue

        all_strikes.extend(df["strike"].tolist())
        all_ttes.extend(df["time_to_expiry"].tolist())
        all_ivs.extend(df["iv_calculated"].tolist())

    if not all_strikes:
        raise ValueError("No valid IV data returned from the options chain.")

    strikes = np.array(all_strikes)
    ttes    = np.array(all_ttes)
    ivs     = np.array(all_ivs)

    fig = plt.figure(figsize=(11, 7), dpi=_FIG_DPI)
    ax  = fig.add_subplot(111, projection="3d")

    sc = ax.scatter(
        strikes, ttes, ivs,
        c=ivs,
        cmap="RdYlGn_r",
        s=18,
        alpha=0.85,
        depthshade=True,
    )

    ax.set_xlabel("Strike ($)", labelpad=8)
    ax.set_ylabel("Time to expiry (yr)", labelpad=8)
    ax.set_zlabel("Implied Volatility", labelpad=8)
    ax.zaxis.set_major_formatter(mticker.PercentFormatter(xmax=1, decimals=0))
    ax.set_title(
        f"{ticker} — {option_type.capitalize()} IV Surface  |  Spot ${spot:,.2f}",
        pad=12, fontsize=12,
    )

    cbar = fig.colorbar(sc, ax=ax, pad=0.10, shrink=0.55)
    cbar.set_label("Implied Vol", fontsize=9)
    cbar.ax.yaxis.set_major_formatter(mticker.PercentFormatter(xmax=1, decimals=0))

    fig.tight_layout()
    return fig


# ---------------------------------------------------------------------------
# CLI demo
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    ticker = sys.argv[1] if len(sys.argv) > 1 else None

    if ticker:
        print(f"Fetching live data for {ticker}…")
        snap = _md.get_market_snapshot(ticker)
        spot = snap.spot
        rfr  = snap.risk_free_rate
        print(f"  Spot=${spot:.2f}  RFR={rfr*100:.2f}%")

        # Fetch nearest expiry to get a representative IV and premium
        chain = _md.get_options_chain(ticker, option_type="call")
        atm = chain.iloc[(chain["strike"] - spot).abs().argsort()].iloc[0]
        strike   = atm["strike"]
        premium  = atm["last_price"]
        vol      = atm["iv_calculated"] if not math.isnan(atm["iv_calculated"]) else 0.20
        tte      = atm["time_to_expiry"]

        print(f"  ATM strike=${strike:.0f}  premium=${premium:.2f}  IV={vol:.1%}  T={tte:.3f}y")
    else:
        # Synthetic demo parameters
        spot, strike, premium = 100.0, 100.0, 5.0
        rfr, vol, tte = 0.05, 0.20, 0.50
        print("No ticker provided — using synthetic demo parameters.")

    print("\nPlotting payoff diagram…")
    fig1 = plot_payoff(
        strike=strike,
        premium=premium,
        option_type="call",
        current_spot=spot if ticker else None,
        title=f"{ticker + ' — ' if ticker else ''}Payoff Diagram",
    )

    print("Plotting Greeks…")
    fig2 = plot_greeks(
        strike=strike,
        time_to_expiration=tte,
        risk_free_rate=rfr,
        volatility=vol,
        current_spot=spot if ticker else None,
        title_prefix=ticker or "Demo",
    )

    if ticker:
        print(f"Plotting IV surface for {ticker} (fetching chain, may take a moment)…")
        try:
            fig3 = plot_vol_surface(ticker)
        except Exception as exc:
            print(f"  IV surface skipped: {exc}")
            fig3 = None

    plt.show()
