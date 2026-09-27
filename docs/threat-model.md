# SecureBootX Threat Model

## 1. Adversary Goals
The primary threat is an adversary attempting to modify a boot component (e.g., inserting a rootkit into the kernel) to gain persistence and control over the system before the OS security policies are active.

## 2. Attack Vectors
- **Disk Modification**: Directly editing `kernel.img` or `bootloader.bin` on the storage medium.
- **Manifest Tampering**: Attempting to modify `manifest.json` to include the hash of a malicious component.
- **Key Replacement**: Trying to replace the trusted public key with an attacker's key.

## 3. Mitigations in SecureBootX
- **Digital Signatures**: Prevent unauthorized modifications to components. Even if the adversary changes the file, they cannot generate a valid signature without the private key.
- **Measured Boot (TPM)**: Even if an adversary bypasses the verification check (e.g., via a vulnerability in the verifier), the TPM PCRs will record the *actual* hash of the malicious component. This allows for "Remote Attestation" where a third party can verify the boot state.
- **Manifest Integrity**: In a real system, the manifest is signed or stored in secure NVRAM. In this simulation, we emphasize the concept of the "Trust Anchor".

## 4. Out of Scope
- Hardware-level attacks (glitching, side-channel).
- Attacks on the TPM hardware itself.
- Modifying the actual physical BIOS/UEFI of the host machine.
