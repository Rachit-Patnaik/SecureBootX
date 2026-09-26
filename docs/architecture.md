# SecureBootX Architecture Specification

## Subsystems

1. **Cryptographic Engine**: Software implementation of SHA-256 digests and RSA-2048 signature verification.
2. **Verified Boot Engine**: Enforces Root-of-Trust verification chain (`Stage 1 SPL` -> `Stage 2 U-Boot` -> `Stage 3 Kernel`).
3. **Measured Boot Engine**: Simulates TPM 2.0 PCR registers (0..23) and computes `PCR_new = SHA256(PCR_old || hash)`.
4. **Trust Framework**: Enforces Monotonic Anti-Rollback Security Version thresholds and key revocation policies.
