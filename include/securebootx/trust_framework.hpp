#ifndef SECUREBOOTX_TRUST_FRAMEWORK_HPP
#define SECUREBOOTX_TRUST_FRAMEWORK_HPP

#include "firmware.hpp"
#include <string>
#include <vector>
#include <unordered_set>

struct SecurityPolicy {
    uint32_t minimum_allowed_version = 1; // Anti-Rollback minimum version threshold
    bool require_rsa_verification = true;
    bool enable_measured_boot = true;
    std::string root_public_key = "DEFAULT_OEM_PUBLIC_KEY";
    std::unordered_set<std::string> revoked_key_fingerprints;
};

class TrustFramework {
public:
    TrustFramework() = default;

    bool loadPolicy(const std::string& config_file);
    bool checkAntiRollback(uint32_t firmware_version);
    bool isKeyRevoked(const std::string& key_fingerprint) const;
    const SecurityPolicy& getPolicy() const { return policy_; }

private:
    SecurityPolicy policy_;
};

#endif // SECUREBOOTX_TRUST_FRAMEWORK_HPP
