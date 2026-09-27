# SecureBootX Cryptography

## 1. Hashing (SHA-256)
SecureBootX uses SHA-256 for all measurements. It provides a 256-bit (32-byte) digest that is computationally infeasible to reverse or collide.
- **Usage**: Every boot component is hashed before being loaded.

## 2. Digital Signatures (RSA/ECDSA)
The project uses asymmetric cryptography to ensure authenticity.
- **Private Key**: Used by the "developer" or "vendor" to sign the boot images. This key must be kept secret.
- **Public Key**: Distributed with the system. Used by the `BootVerifier` to check that the signature was created by the holder of the private key.

## 3. Implementation Details
The project uses the **OpenSSL EVP (Envelope)** API. EVP is the modern, recommended interface for OpenSSL as it provides:
- High-level abstraction over specific algorithms.
- Better support for hardware acceleration.
- Easier migration between algorithms (e.g., switching from RSA to ECDSA).

## 4. Key Management
Keys are stored in the `keys/` directory. 
- `private_key.pem`: (Generated locally) Must NOT be committed to Git.
- `public_key.pem`: (Generated locally) Used for verification.
