#include "boot/manifest.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace securebootx {

/**
 * NOTE: For a production system, we would use a proper JSON library like nlohmann/json.
 * To keep dependencies minimal as requested, I am implementing a simple
 * JSON-like parser/serializer for the manifest.
 */

bool Manifest::load(const std::filesystem::path& manifestPath) {
    std::ifstream file(manifestPath);
    if (!file) {
        std::cerr << "[Manifest] Error: Could not open " << manifestPath << std::endl;
        return false;
    }

    std::string line;
    bool inComponent = false;
    BootComponent currentComp;

    while (std::getline(file, line)) {
        // Remove leading whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));

        if (line.find("\"components\": [") != std::string::npos) {
            inComponent = true;
            continue;
        }
        if (inComponent && line.find("]") != std::string::npos) {
            inComponent = false;
            continue;
        }
        if (inComponent && line.find("{") != std::string::npos) {
            currentComp = BootComponent();
            continue;
        }
        if (inComponent && line.find("}") != std::string::npos) {
            components.push_back(currentComp);
            continue;
        }
        if (inComponent) {
            size_t quote1 = line.find("\"");
            size_t quote2 = line.find("\"", quote1 + 1);
            if (quote1 == std::string::npos || quote2 == std::string::npos) continue;

            std::string key = line.substr(quote1 + 1, quote2 - quote1 - 1);
            size_t colon = line.find(":", quote2);
            if (colon == std::string::npos) continue;

            size_t valQuote1 = line.find("\"", colon);
            size_t valQuote2 = line.find("\"", valQuote1 + 1);

            if (valQuote1 != std::string::npos && valQuote2 != std::string::npos) {
                std::string val = line.substr(valQuote1 + 1, valQuote2 - valQuote1 - 1);
                if (key == "name") currentComp.name = val;
                else if (key == "path") currentComp.path = val;
                else if (key == "type") currentComp.type = stringToComponentType(val);
                else if (key == "sha256") currentComp.expectedHash = val;
                else if (key == "signature") currentComp.signaturePath = val;
            } else {
                // Handle numeric values like "pcr": 0
                size_t endOfVal = line.find_first_of(",\n\r", colon + 1);
                std::string val = line.substr(colon + 1, endOfVal - colon - 1);
                // Trim whitespace
                val.erase(0, val.find_first_not_of(" \t"));
                val.erase(val.find_last_not_of(" \t") + 1);
                if (key == "pcr") currentComp.pcrIndex = std::stoi(val);
            }
        }
        if (line.find("\"publicKey\":") != std::string::npos) {
            size_t q1 = line.find("\"", line.find(":"));
            size_t q2 = line.find("\"", q1 + 1);
            if (q1 != std::string::npos && q2 != std::string::npos) {
                publicKeyPath = line.substr(q1 + 1, q2 - q1 - 1);
            }
        }
    }
    return !components.empty();
}

void Manifest::save(const std::filesystem::path& manifestPath) const {
    std::ofstream file(manifestPath);
    file << "{\n";
    file << "  \"version\": \"" << version << "\",\n";
    file << "  \"algorithm\": \"" << algorithm << "\",\n";
    file << "  \"publicKey\": \"" << publicKeyPath << "\",\n";
    file << "  \"components\": [\n";
    for (size_t i = 0; i < components.size(); ++i) {
        const auto& c = components[i];
        file << "    {\n";
        file << "      \"name\": \"" << c.name << "\",\n";
        file << "      \"type\": \"" << componentTypeToString(c.type) << "\",\n";
        file << "      \"path\": \"" << c.path.string() << "\",\n";
        file << "      \"sha256\": \"" << c.expectedHash << "\",\n";
        file << "      \"signature\": \"" << c.signaturePath.string() << "\",\n";
        file << "      \"pcr\": " << c.pcrIndex << "\n";
        file << "    }" << (i == components.size() - 1 ? "" : ",") << "\n";
    }
    file << "  ]\n";
    file << "}\n";
}

void Manifest::addComponent(const BootComponent& component) {
    components.push_back(component);
}

} // namespace securebootx
