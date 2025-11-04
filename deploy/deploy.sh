#!/usr/bin/env bash
set -euo pipefail
ssh dog 'cd ~/robot-voice-dog && git pull && git submodule update --init --recursive && \
         cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j$(nproc) && \
         sudo systemctl restart robot-voice && systemctl is-active --quiet robot-voice && echo "robot-voice activo"'
