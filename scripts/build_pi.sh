#!/usr/bin/env bash
set -euo pipefail
cd ~/robot-voice-dog
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
