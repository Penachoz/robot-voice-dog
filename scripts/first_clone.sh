#!/usr/bin/env bash
set -euo pipefail
read -rp "Tu usuario de GitHub (TU_USER): " GH
ssh dog "test -d ~/robot-voice-dog || git clone git@github.com:${GH}/robot-voice-dog.git ~/robot-voice-dog && \
         cd ~/robot-voice-dog && git submodule update --init --recursive"
