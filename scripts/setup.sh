#!/bin/bash

# Setup dependencies for WSL2 Debian
echo "[SecureBootX] Installing dependencies..."
sudo apt-get update
sudo apt-get install -y cmake build-essential libssl-dev pkg-config libgtest-dev

echo "[SecureBootX] Dependencies installed successfully."
