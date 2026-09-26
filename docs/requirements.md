# SecureBootX Requirements Specification

## Functional Requirements

1. **Firmware Verification**: Verify SHA-256 binary hash and RSA-2048 signature of every boot stage header.
2. **Measured Boot Attestation**: Compute stage hashes and extend TPM 2.0 PCR registers with TCG log entries.
3. **Anti-Rollback Protection**: Prevent flashing or booting images with a version number less than the policy threshold.
4. **CLI Management Interface**: Command line tool supporting `run`, `sign`, `verify`, and `measure`.
