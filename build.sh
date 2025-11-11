#!/usr/bin/env bash
set -euo pipefail

APP_NAME="Lab01_MaHoa"
BUILD_DIR="build"
CONFIG="${1:-Release}"   # ./build.sh [Debug|Release]

# Chọn generator
GEN=""
if command -v g++ >/dev/null 2>&1; then
  GEN='-G "MinGW Makefiles"'
elif command -v ninja >/dev/null 2>&1; then
  GEN='-G Ninja'
else
  GEN='-G "Visual Studio 17 2022" -A x64'
fi

# Configure
cmake -S . -B "$BUILD_DIR" $GEN

# Build đúng target main của bạn
cmake --build "$BUILD_DIR" --config "$CONFIG" --target "$APP_NAME" -- -j

# Chạy
echo "▶️  Run $APP_NAME"
if [[ -f "$BUILD_DIR/$APP_NAME.exe" ]]; then
  "$BUILD_DIR/$APP_NAME.exe"
elif [[ -f "$BUILD_DIR/$CONFIG/$APP_NAME.exe" ]]; then
  "$BUILD_DIR/$CONFIG/$APP_NAME.exe"
elif [[ -f "$BUILD_DIR/$APP_NAME" ]]; then
  "$BUILD_DIR/$APP_NAME"
else
  "$BUILD_DIR/$CONFIG/$APP_NAME" || true
fi
