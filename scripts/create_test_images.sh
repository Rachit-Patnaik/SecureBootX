#!/bin/bash

# Create dummy binary files for the demo
echo "[SecureBootX] Creating demo boot components..."
mkdir -p demo data

# Create dummy images
echo "Dummy Firmware Content" > demo/firmware.bin
echo "Dummy Bootloader Content" > demo/bootloader.bin
echo "Dummy Kernel Content" > demo/kernel.img
echo "Dummy Initramfs Content" > demo/initramfs.img
echo "Dummy RootFS Content" > demo/rootfs.img

echo "[+] Demo images created in demo/ directory."
