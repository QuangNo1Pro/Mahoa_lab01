#!/usr/bin/env bash
# tests.sh — Build & run all *.spec.cpp via CTest, keep window open
set -Eeuo pipefail

BUILD_DIR="build"
CONFIG="${1:-Release}"             # ./tests.sh [Debug|Release]
LOG_FILE="$BUILD_DIR/test.log"

# ---- helpers ----
pause_now() {
  # pause nếu đang chạy trong terminal và không tắt bằng biến NO_PAUSE
  if [[ -t 0 && "${NO_PAUSE:-0}" != "1" ]]; then
    echo
    read -r -p "Press ENTER to close..." _
  fi
}
on_error() {
  code=$?
  echo -e "\n❌ tests.sh failed (exit $code) at line $LINENO: $BASH_COMMAND"
  if [[ -f "$LOG_FILE" ]]; then
    echo -e "\n--- Last 80 lines of $LOG_FILE ---"
    tail -n 80 "$LOG_FILE" || true
    echo -e "----------------------------------\n"
  fi
  pause_now
  exit $code
}
trap on_error ERR

# ---- pick generator as ARGS (để không vỡ token) ----
GEN_ARGS=()
if command -v g++ >/dev/null 2>&1; then
  GEN_ARGS=(-G "MinGW Makefiles")
elif command -v ninja >/dev/null 2>&1; then
  GEN_ARGS=(-G Ninja)
else
  GEN_ARGS=(-G "Visual Studio 17 2022" -A x64)
fi
echo "🛠  Generator: ${GEN_ARGS[*]:-default} | Config: $CONFIG"

mkdir -p "$BUILD_DIR"

# ---- configure (idempotent) ----
cmake -S . -B "$BUILD_DIR" "${GEN_ARGS[@]}"

# ---- build tests target (gom tất cả *.spec.cpp) ----
# Nếu bạn chưa làm add_custom_target(tests_all ...) trong tests/CMakeLists.txt,
# bước này sẽ fail — hãy thêm nó như hướng dẫn trước đó.
cmake --build "$BUILD_DIR" --config "$CONFIG" --target tests_all -- -j

# ---- run ctest (verbose) + log ----
echo "🧪 Running tests... (log: $LOG_FILE)"
set +e
ctest --test-dir "$BUILD_DIR" -C "$CONFIG" -VV --output-on-failure 2>&1 | tee "$LOG_FILE"
STATUS=${PIPESTATUS[0]}
set -e

if (( STATUS == 0 )); then
  echo "✅ All tests passed."
else
  echo "❌ Some tests failed. See full log at: $LOG_FILE"
fi

pause_now
exit $STATUS
