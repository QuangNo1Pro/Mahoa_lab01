g++ --version#!/bin/bash

# ============================
# 🧩 Script build CMake project (cross-compiler safe)
# ============================

APP_NAME="Lab01_MaHoa"

# Detect g++
if command -v g++ >/dev/null 2>&1; then
  GENERATOR="MinGW Makefiles"
  echo "🛠 Detected g++ — using generator: $GENERATOR"
else
  GENERATOR=""
  echo "No g++ found — using default generator (Visual Studio/MSBuild)"
fi

# Create build directory if missing
if [ ! -d "build" ]; then
  mkdir build
fi

# Configure CMake
if [ -n "$GENERATOR" ]; then
  cmake -B build -G "$GENERATOR"
else
  cmake -B build
fi

# Build project
cmake --build build

# Run program if build successful
if [ $? -eq 0 ]; then
  echo -e "\n Build successful! Running program...\n"
  echo "------------------- Output -------------------"
  echo ""
  if [ -f "./build/$APP_NAME.exe" ]; then
    ./build/$APP_NAME.exe
  elif [ -f "./build/Debug/$APP_NAME.exe" ]; then
    ./build/Debug/$APP_NAME.exe
  else
    echo " Cannot find $APP_NAME.exe (check build output folder)"
  fi
else
  echo "Build failed!"
fi
