#!/usr/bin/env bash
set -euo pipefail
read -rp "IP de la Raspberry (ej. 192.168.1.23): " PI_IP
read -rp "Usuario en la Raspberry (ej. pi): " PI_USER

mkdir -p ~/.ssh
if [ ! -f ~/.ssh/id_ed25519 ]; then
  ssh-keygen -t ed25519 -C "pc-dev" -N "" -f ~/.ssh/id_ed25519
fi

if ! grep -q "^Host dog$" ~/.ssh/config 2>/dev/null; then
cat >> ~/.ssh/config <<EOF2
Host dog
    HostName ${PI_IP}
    User ${PI_USER}
    IdentitiesOnly yes
    IdentityFile ~/.ssh/id_ed25519
    ServerAliveInterval 30
    ServerAliveCountMax 4
EOF2
fi

ssh-copy-id dog || true
ssh dog 'echo "OK conexion Pi"'
