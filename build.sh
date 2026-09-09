#!/usr/bin/env bash

set -e

BUILD_DIR="build"

cmake -S . -B "$BUILD_DIR" \
-G "Visual Studio 17 2022" \
-T ClangCL

cmake --build "$BUILD_DIR" --config Release
