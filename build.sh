#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="build"

# Check required tools.
if ! command -v cmake >/dev/null 2>&1; then
    echo "Error: CMake was not found in PATH."
    exit 1
fi

if ! command -v ninja >/dev/null 2>&1; then
    echo "Error: Ninja was not found in PATH."
    exit 1
fi

if ! command -v clang++ >/dev/null 2>&1; then
    echo "Error: clang++ was not found in PATH."
    exit 1
fi

# Find clang++ using the Git Bash PATH and convert the path
# to Windows format for CMake.
CLANGXX_UNIX="$(command -v clang++)"
CLANGXX="$(cygpath -w "$CLANGXX_UNIX")"

if [[ -z "$VCPKG_ROOT" ]]; then
    echo "Error: VCPKG_ROOT is not set."
    exit 1
fi

VCPKG_TOOLCHAIN="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

if [[ ! -f "$VCPKG_TOOLCHAIN" ]]; then
    echo "Error: vcpkg toolchain file not found:"
    echo "$VCPKG_TOOLCHAIN"
    exit 1
fi

echo "Using Clang++: $CLANGXX"
echo "Using vcpkg: $VCPKG_ROOT"

rm -rf "$BUILD_DIR"

cmake -S . -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_CXX_COMPILER="$CLANGXX" \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN"

cmake --build "$BUILD_DIR"
