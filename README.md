# SecureBootX — Verified Boot, Measured Boot & Firmware Trust Framework

SecureBootX is an embedded security framework implemented in modern C++17 designed to guarantee firmware integrity and platform trustworthiness during boot. It combines **Verified Boot** (Static Chain of Trust verification via SHA-256 digests and RSA-2048 signatures), **Measured Boot** (TPM 2.0 PCR extensions and TCG event logging), and **Firmware Trust Policies** (Anti-Rollback version protection and key management).

---

## Architecture Overview

```text
+-------------------------------------------------------------+
|                     SecureBootX CLI                         |
+------------------------------+------------------------------+
                               |
                               v
+-------------------------------------------------------------+
|                   Verified Boot Engine                      |
| (Root of Trust -> SPL -> Main Bootloader -> Linux Kernel)  |
+------------------------------+------------------------------+
                               |
                               v
+-------------------------------------------------------------+
|                   Measured Boot Engine                      |
|   (TPM 2.0 PCR Extension: PCR_new = SHA256(PCR_old || hash))|
+-------------------------------------------------------------+
```

---

## Directory Structure

```text
SecureBootX/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── .gitignore
├── include/
│   └── securebootx/
│       ├── crypto_engine.hpp
│       ├── verified_boot.hpp
│       ├── measured_boot.hpp
│       ├── trust_framework.hpp
│       ├── firmware.hpp
│       └── logger.hpp
├── src/
│   ├── main.cpp
│   ├── crypto_engine.cpp
│   ├── verified_boot.cpp
│   ├── measured_boot.cpp
│   ├── trust_framework.cpp
│   ├── firmware.cpp
│   └── logger.cpp
├── configs/
│   └── policy.json
├── firmware_images/
│   ├── stage1_spl.bin
│   ├── stage2_uboot.bin
│   └── stage3_kernel.bin
├── tests/
│   ├── crypto_test.cpp
│   ├── verified_boot_test.cpp
│   └── measured_boot_test.cpp
└── docs/
    ├── architecture.md
    ├── requirements.md
    └── screenshots/
```

---

## Building & Verification

```bash
mkdir -p build && cd build
cmake ..
make -j$(nproc)
ctest --output-on-failure
```

---

## Usage Guide

### 1. Execute Complete Verified & Measured Boot Sequence
```bash
./securebootx run ../configs/policy.json
```

### 2. Sign a Custom Firmware Image
```bash
./securebootx sign raw_payload.bin stage1_spl.bin 1 1
```

### 3. Verify Firmware Binary
```bash
./securebootx verify firmware_images/stage1_spl.bin ../configs/policy.json
```

### 4. Extend TPM 2.0 PCR Measurement
```bash
./securebootx measure firmware_images/stage1_spl.bin
```
