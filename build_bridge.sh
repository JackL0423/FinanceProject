#!/usr/bin/env bash
# build_bridge.sh — Build the pricing_core Python extension.
#
# Usage:
#   ./build_bridge.sh              # auto-detects active Python / conda env
#   ./build_bridge.sh finance      # activate the 'finance' conda env first
#
# After a successful build, pricing_core.so lands in build/ and
# pricing.py can be used from this directory.

set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ── Optional: activate a named conda env ─────────────────────────────────────
CONDA_ENV="${1:-}"
if [[ -n "$CONDA_ENV" ]]; then
    # Find conda init script
    CONDA_BASE=$(conda info --base 2>/dev/null || true)
    if [[ -z "$CONDA_BASE" ]]; then
        # Common default locations
        for candidate in "$HOME/anaconda3" "$HOME/miniconda3" \
                         "/opt/anaconda3" "/opt/miniconda3"; do
            [[ -d "$candidate" ]] && CONDA_BASE="$candidate" && break
        done
    fi
    if [[ -z "$CONDA_BASE" ]]; then
        echo "ERROR: conda not found. Activate '$CONDA_ENV' manually and re-run." >&2
        exit 1
    fi
    # shellcheck source=/dev/null
    source "$CONDA_BASE/etc/profile.d/conda.sh"
    conda activate "$CONDA_ENV"
    echo "Activated conda env: $CONDA_ENV ($(which python))"
fi

PYTHON=$(which python3 2>/dev/null || which python)
echo "Using Python: $PYTHON ($($PYTHON --version))"

# ── Install pybind11 if missing ───────────────────────────────────────────────
if ! $PYTHON -c "import pybind11" 2>/dev/null; then
    echo "pybind11 not found — installing..."
    $PYTHON -m pip install pybind11
fi
PYBIND11_DIR=$($PYTHON -c "import pybind11; print(pybind11.get_cmake_dir())")
echo "pybind11 cmake dir: $PYBIND11_DIR"

# ── Wipe stale cache if source path has changed ───────────────────────────────
BUILD_DIR="$SCRIPT_DIR/build"
CACHE="$BUILD_DIR/CMakeCache.txt"
if [[ -f "$CACHE" ]] && ! grep -q "$(printf '%s' "$SCRIPT_DIR")" "$CACHE" 2>/dev/null; then
    echo "Stale CMake cache detected (built in a different location) — clearing build/..."
    rm -rf "$BUILD_DIR"
fi

# ── Configure ─────────────────────────────────────────────────────────────────
cmake -B "$BUILD_DIR" \
      -S "$SCRIPT_DIR" \
      -DCMAKE_BUILD_TYPE=Release \
      -Dpybind11_DIR="$PYBIND11_DIR" \
      -DPYTHON_EXECUTABLE="$PYTHON"

# ── Build ─────────────────────────────────────────────────────────────────────
cmake --build "$BUILD_DIR" --target pricing_core -j"$(nproc 2>/dev/null || sysctl -n hw.logicalcpu)"

echo ""
echo "Build complete. pricing_core.so is in: $BUILD_DIR"
echo ""
echo "To use it:"
echo "  export PYTHONPATH=\"$BUILD_DIR:\$PYTHONPATH\""
echo "  python pricing.py"
