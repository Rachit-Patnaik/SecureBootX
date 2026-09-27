# SecureBootX Boot Flow

This document describes the sequential process of establishing trust during the simulated boot process.

## 1. The Chain of Trust Sequence

### Stage 0: Root of Trust (RoT)
The process begins with a Root of Trust. In a real system, this is the immutable ROM in the CPU/SoC. In SecureBootX, the RoT is the trusted public key and the initial manifest.

### Stage 1: Firmware Verification
- **Measure**: Calculate SHA-256 of `firmware.bin`.
- **Verify**: Check signature against the trusted key.
- **Extend**: Extend hash into TPM PCR 0.
- **Decision**: If failure $\rightarrow$ Block Boot.

### Stage 2: Bootloader Verification
- **Measure**: Calculate SHA-256 of `bootloader.bin`.
- **Verify**: Check signature.
- **Extend**: Extend hash into TPM PCR 4.
- **Decision**: If failure $\rightarrow$ Block Boot.

### Stage 3: Kernel Verification
- **Measure**: Calculate SHA-256 of `kernel.img`.
- **Verify**: Check signature.
- **Extend**: Extend hash into TPM PCR 4.
- **Decision**: If failure $\rightarrow$ Block Boot.

### Stage 4: Initramfs & RootFS Verification
- **Measure**: Calculate SHA-256 of `initramfs.img` and `rootfs.img`.
- **Verify**: Check signatures.
- **Extend**: Extend hashes into TPM PCR 9.
- **Decision**: If failure $\rightarrow$ Block Boot.

## 2. Measured Boot vs Verified Boot
| Feature | Verified Boot | Measured Boot |
| :--- | :--- | :--- |
| **Goal** | Prevention (don't run bad code) | Detection (prove what ran) |
| **Mechanism** | Digital Signatures | Hashing + TPM PCRs |
| **Action** | Stop boot on failure | Log measurement for remote attestation |
| **Trust Anchor** | Public Key | TPM Endorsement Key / PCRs |
