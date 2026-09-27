#!/bin/bash

# SecureBootX Installation Script
# This script installs the binary and the systemd service.

echo "[SecureBootX] Starting installation..."

# 1. Install binary
sudo cp build/securebootx /usr/local/bin/securebootx
echo "[+] Binary installed to /usr/local/bin/securebootx"

# 2. Install systemd service
sudo cp service/securebootx.service /etc/systemd/system/securebootx.service
echo "[+] Systemd service installed to /etc/systemd/system/securebootx.service"

# 3. Reload systemd
sudo systemctl daemon-reload
echo "[+] Systemd daemon reloaded"

# 4. Enable service
sudo systemctl enable securebootx.service
echo "[+] Service enabled to run at boot"

echo "\nInstallation complete. You can now run: sudo systemctl start securebootx"
