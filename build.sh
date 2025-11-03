#!/bin/bash

# ============================
# 🧩 Script build CMake project
# ============================

APP_NAME="Lab01_MaHoa"

if [ ! -d "build" ]; then
  mkdir build
fi

cmake -B build

cmake --build build

if [ $? -eq 0 ]; then
  echo -e "Build successful! Running program...\n"
  echo "------------------- Output -------------------"
  echo ""
  ./build/$APP_NAME.exe
else
  echo "Build failed!"
fi
