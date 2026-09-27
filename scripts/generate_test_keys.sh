#!/bin/bash

# Key generation script
echo "[SecureBootX] Generating RSA key pair..."
mkdir -p keys

# Generate private key
openssl genrsa -out keys/private_key.pem 2048
# Extract public key
openssl rsa -in keys/private_key.pem -pubout -out keys/public_key.pem

echo "[+] Keys generated in keys/ directory."
