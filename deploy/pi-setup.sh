#!/usr/bin/env bash
set -euo pipefail
ssh dog 'sudo apt-get update && sudo apt-get install -y \
  git build-essential cmake pkg-config \
  portaudio19-dev libasound2-dev alsa-utils wget'
ssh dog 'sudo usermod -aG audio $USER || true'
