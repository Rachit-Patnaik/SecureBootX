#include "crypto/signer.hpp"
#include "crypto/verifier.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: sbx-keygen <output_dir>\n";
        return 1;
    }

    std::string outputDir = argv[1];
    std::filesystem::create_directories(outputDir);

    // In this educational project, we use a shell script for the heavy lifting
    // of OpenSSL keygen for simplicity, but this tool provides the CLI entry point.
    std::cout << "Key generation requested for: " << outputDir << "\n";
    std::cout << "Please run './scripts/generate_test_keys.sh' to generate valid PEM keys.\n";

    return 0;
}
