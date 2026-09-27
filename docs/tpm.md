# SecureBootX TPM Integration

## 1. TPM 2.0 Concepts
The Trusted Platform Module (TPM) is a secure microcontroller that provides a hardware root of trust.

### PCRs (Platform Configuration Registers)
PCRs are unique registers that cannot be "set" to a value. They can only be **extended**.
The extension operation is:
$PCR_{new} = SHA256(PCR_{old} \ || \ NewMeasurement)$

This means the final value of a PCR depends on the exact order and content of everything measured into it.

## 2. PCR Mapping in SecureBootX
| PCR Index | Component | Description |
| :--- | :--- | :--- |
| 0 | Firmware | Core System Firmware / BIOS |
| 4 | Bootloader/Kernel | Bootloader and OS Kernel |
| 9 | Initramfs/RootFS | Initial RAM disk and Root filesystem |

## 3. WSL2 vs Native Linux
### WSL2
WSL2 does not provide a virtual TPM by default in all configurations. SecureBootX detects this and falls back to **Software Measurement Mode**. In this mode, we simulate the PCR extension in memory.

### Native Linux
On native hardware with a TPM 2.0 chip, SecureBootX uses the `tpm2-tss` library to perform real hardware extensions and reads.

## 4. Hardware Requirements
- TPM 2.0 compliant chip.
- Linux kernel with `/dev/tpm0` or `/dev/tpmrm0` available.
- `tpm2-tss` libraries installed.
