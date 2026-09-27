# SecureBootX Testing Strategy

## 1. Unit Testing
Each module has corresponding tests in the `tests/` directory using GoogleTest/CTest.
- `test_hash`: Verifies SHA-256 correctness against known vectors.
- `test_signature`: Verifies that valid signatures pass and tampered signatures fail.
- `test_manifest`: Ensures JSON parsing of the trust baseline is robust.
- `test_trust_manager`: Tests the logic of the trust decision engine.

## 2. Functional Testing (The Demo)
The project includes a set of scripts to automate the demonstration of the security model:
1. `create_test_images.sh`: Creates dummy binary files for the boot chain.
2. `generate_test_keys.sh`: Generates the RSA key pair.
3. `run_demo.sh`: Executes a full `boot-check` on a clean system.
4. `simulate_tamper.sh`: Modifies a component and proves the system detects it.

## 3. Test Matrix
| Scenario | Expected Result | Verification Method |
| :--- | :--- | :--- |
| Clean Boot | `TRUSTED` | `boot-check` |
| Modified Kernel | `UNTRUSTED` | `boot-check` $\rightarrow$ Hash Mismatch |
| Corrupted Signature | `UNTRUSTED` | `boot-check` $\rightarrow$ Signature Invalid |
| Missing Component | `UNTRUSTED` | `boot-check` $\rightarrow$ File Not Found |
| TPM Unavailable | `TRUSTED` (SW Mode) | `tpm status` $\rightarrow$ Software |
