#include "crypto/verifier.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: sbx-verify <file> <signature_file> <public_key_path>\n";
        return 1;
    }

    try {
        std::string filePath = argv[1];
        std::string sigPath = argv[2];
        std::string keyPath = argv[3];

        if (securebootx::Verifier::verifyFile(filePath, sigPath, keyPath)) {
            std::cout << "Signature VALID\n";
        } else {
            std::cout << "Signature INVALID\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
