#!/bin/bash

# Complete Demo Workflow
echo "===================================================="
echo "       SECUREBOOTX DEMO: CHAIN OF TRUST"
echo "===================================================="

# 1. Setup
./scripts/create_test_images.sh
./scripts/generate_test_keys.sh

# 2. Sign images
echo -e "\n[1/4] Signing boot components..."
./build/securebootx sign demo/firmware.bin
./build/securebootx sign demo/bootloader.bin
./build/securebootx sign demo/kernel.img
./build/securebootx sign demo/initramfs.img
./build/securebootx sign demo/rootfs.img

# 3. Create Manifest
echo -e "\n[2/4] Generating trusted manifest..."
mkdir -p data

# Calculate hashes and build manifest.json
# We use a strictly formatted JSON that our parser expects
cat <<EOF > data/manifest.json
{
  "version": "1.0",
  "publicKey": "keys/public_key.pem",
  "components": [
    {
      "name": "Firmware",
      "type": "firmware",
      "path": "demo/firmware.bin",
      "sha256": "$(sha256sum demo/firmware.bin | cut -d' ' -f1)",
      "signature": "demo/firmware.bin.sig",
      "pcr": 0
    },
    {
      "name": "Bootloader",
      "type": "bootloader",
      "path": "demo/bootloader.bin",
      "sha256": "$(sha256sum demo/bootloader.bin | cut -d' ' -f1)",
      "signature": "demo/bootloader.bin.sig",
      "pcr": 4
    },
    {
      "name": "Kernel",
      "type": "kernel",
      "path": "demo/kernel.img",
      "sha256": "$(sha256sum demo/kernel.img | cut -d' ' -f1)",
      "signature": "demo/kernel.img.sig",
      "pcr": 4
    },
    {
      "name": "Initramfs",
      "type": "initramfs",
      "path": "demo/initramfs.img",
      "sha256": "$(sha256sum demo/initramfs.img | cut -d' ' -f1)",
      "signature": "demo/initramfs.img.sig",
      "pcr": 9
    },
    {
      "name": "RootFS",
      "type": "rootfs",
      "path": "demo/rootfs.img",
      "sha256": "$(sha256sum demo/rootfs.img | cut -d' ' -f1)",
      "signature": "demo/rootfs.img.sig",
      "pcr": 9
    }
  ]
}
EOF

# 4. Run Initial Boot Check
echo -e "\n[3/4] Running initial boot-check (Expected: TRUSTED)..."
./build/securebootx boot-check

# 5. Simulate Attack
echo -e "\n[4/4] Simulating attack on Kernel..."
./build/securebootx simulate-tamper "Kernel"
echo -e "\nRunning boot-check after attack (Expected: UNTRUSTED)..."
./build/securebootx boot-check

# 6. Restore and Verify
echo -e "\n[Final] Restoring and verifying..."
./build/securebootx restore-demo
./build/securebootx boot-check
