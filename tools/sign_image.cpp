#include "crypto/signer.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: sbx-sign <file_to_sign> <private_key_path>\n";
        return 1;
    }

    try {
        std::string filePath = argv[1];
        std::string keyPath = argv[2];
        std::string sigPath = filePath + ".sig";

        if (securebootx::Signer::signFile(filePath, keyPath, sigPath)) {
            std::cout << "Successfully signed " << filePath << " -> " << sigPath << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
