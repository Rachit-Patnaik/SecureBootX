# SecureBootX Project Architecture

## 1. High-Level Design
SecureBootX implements a layered approach to boot integrity. It separates the *verification* (is it signed?) from the *measurement* (what is it?).

### Verified Boot (Authenticity)
Uses asymmetric cryptography. Each component is signed with a private key. The system verifies the signature using a public key stored in a trusted manifest.

### Measured Boot (Identity)
Uses a cryptographic hash (SHA-256). Each component's hash is "extended" into a Platform Configuration Register (PCR) in the TPM. This creates an immutable record of the boot sequence.

## 2. Module Breakdown

### `crypto`
- `Hash`: Wraps OpenSSL EVP for SHA-256.
- `Signer/Verifier`: Handles RSA/ECDSA key loading and signature verification.

### `boot`
- `BootComponent`: Data structure representing a boot stage (Firmware, Kernel, etc.).
- `Manifest`: JSON-based trusted baseline containing expected hashes and keys.

### `measurement`
- `MeasurementEngine`: Calculates hashes and manages the boot log.
- `MeasurementLog`: Persists measurements to a JSON file.

### `tpm`
- `TpmManager`: Interface to `tpm2-tss`.
- `PcrManager`: Handles PCR reads and extensions.

### `trust`
- `TrustManager`: The decision engine. It aggregates results from all components and the TPM to decide if the system is `TRUSTED` or `UNTRUSTED`.

## 3. Trust Decision Logic
A component is considered `TRUSTED` if:
1. Its current SHA-256 matches the manifest.
2. Its digital signature is valid.
3. (Optional) Its measurement is correctly extended into the TPM PCR.

The system state is `TRUSTED` only if all critical components are `TRUSTED`.
