# SecureBootX: Verified Boot, Measured Boot & Firmware Trust Framework

SecureBootX is an educational capstone project designed to demonstrate the concepts of a hardware-rooted chain of trust, verified boot, and measured boot. It simulates how modern systems ensure that only authentic code is executed and that the boot process is accurately recorded.

## 🎯 Project Objective
The goal is to simulate a complete chain-of-trust from firmware up to the root filesystem, ensuring that each component is authentic (Verified Boot) and that its identity is recorded (Measured Boot).

### Chain of Trust Flow:
`Firmware` $\rightarrow$ `Bootloader` $\rightarrow$ `Linux Kernel` $\rightarrow$ `Initramfs` $\rightarrow$ `Root Filesystem` $\rightarrow$ `Linux`

## 🛠 Technology Stack
- **Language:** C++17
- **Build System:** CMake
- **Cryptography:** OpenSSL (libcrypto)
- **Platform:** Linux / WSL2 Debian
- **Hardware Anchor:** TPM 2.0 (via Software Simulation for WSL2 / `tpm2-tss` for Native Linux)

## 🏗 Architecture
The project is divided into several core modules:
- **Crypto:** Handles SHA-256 hashing and RSA/ECDSA signatures.
- **Boot:** Manages boot components and the trusted manifest (baseline).
- **Measurement:** Implements the measurement engine and boot log.
- **TPM:** Provides an abstraction layer for TPM 2.0 PCR operations.
- **Trust:** Evaluates the overall trust state based on strict or permissive policies.
- **Simulation:** Safely simulates tampering with test images to demonstrate detection.

## 🚀 Getting Started

### Prerequisites
Ensure you have the following installed in your WSL2/Linux environment:
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential libssl-dev pkg-config libgtest-dev
```

### Build Instructions
```bash
# Configure the project
cmake -S . -B build

# Build all binaries and tests
cmake --build build -j$(nproc)
```

### 🎬 Running the Demo
The fastest way to see the system in action is to use the automated demo script, which handles image creation, key generation, signing, and attack simulation:
```bash
chmod +x scripts/*.sh
./scripts/run_demo.sh
```

### 🛠 Manual Usage
If you prefer to test individual components:

1. **Setup Environment**:
   ```bash
   ./scripts/create_test_images.sh
   ./scripts/generate_test_keys.sh
   ```

2. **Sign Components**:
   ```bash
   ./build/securebootx sign demo/kernel.img
   ```

3. **Perform Boot Check**:
   ```bash
   ./build/securebootx boot-check
   ```

4. **Simulate an Attack**:
   ```bash
   ./build/securebootx simulate-tamper "Kernel"
   ./build/securebootx boot-check
   ```

5. **Restore and Verify**:
   ```bash
   ./build/securebootx restore-demo
   ./build/securebootx boot-check
   ```

## 🧪 Testing
The project includes a comprehensive test suite. To run all tests:
```bash
cd build
ctest --output-on-failure
```

## ⚠️ Security Note
This is an **educational project**. It operates on simulated images in the `demo/` directory and **never** modifies your actual system bootloader, EFI partition, or kernel.

## 📖 Documentation
Detailed design documents are available in the `docs/` directory:
- `architecture.md`: High-level system design.
- `boot-flow.md`: Step-by-step chain of trust sequence.
- `threat-model.md`: Analysis of attack vectors and mitigations.
- `cryptography.md`: Details on SHA-256 and RSA implementations.
- `tpm.md`: Explanation of PCRs and TPM simulation.
- `testing.md`: Testing strategy and matrix.
