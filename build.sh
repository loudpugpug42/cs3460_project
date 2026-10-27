#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="build"

for tool in cmake ninja clang++; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "Error: $tool was not found in PATH."
        echo "Please install $tool and make it available in PATH."
        exit 1
    fi
done

if [[ -z "${VCPKG_ROOT:-}" ]]; then
    echo "Error: VCPKG_ROOT is not set."
    echo "Set it to the location of your vcpkg installation."
    exit 1
fi

VCPKG_TOOLCHAIN="${VCPKG_ROOT%/}/scripts/buildsystems/vcpkg.cmake"

if [[ ! -f "$VCPKG_TOOLCHAIN" ]]; then
    echo "Error: vcpkg CMake toolchain was not found:"
    echo "$VCPKG_TOOLCHAIN"
    exit 1
fi

echo "Using C++ compiler: $(clang++ --version | head -n 1)"
echo "Using CMake: $(cmake --version | head -n 1)"
echo "Using Ninja: $(ninja --version)"
echo "Using vcpkg root: $VCPKG_ROOT"

cmake -S . -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN"

cmake --build "$BUILD_DIR" --parallel
