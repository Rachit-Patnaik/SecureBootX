cmake_minimum_required(VERSION 3.15)

project(SecureBootX
    VERSION 1.0.0
    DESCRIPTION "Verified Boot, Measured Boot and Firmware Trust Framework"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenSSL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/common/cli.cpp
    src/crypto/hash.cpp
    src/crypto/signer.cpp
    src/crypto/verifier.cpp
    src/boot/boot_component.cpp
    src/boot/boot_chain.cpp
    src/boot/boot_verifier.cpp
    src/boot/manifest.cpp
    src/measurement/measurement.cpp
    src/measurement/measurement_engine.cpp
    src/measurement/measurement_log.cpp
    src/tpm/tpm_manager.cpp
    src/tpm/pcr_manager.cpp
    src/trust/trust_policy.cpp
    src/trust/trust_manager.cpp
    src/simulation/tamper_simulator.cpp
    src/common/logger.cpp
    src/common/config.cpp
)

add_executable(securebootx ${SOURCES})
target_link_libraries(securebootx PRIVATE OpenSSL::Crypto)

add_executable(sbx-keygen tools/keygen.cpp src/common/logger.cpp src/common/config.cpp)
target_link_libraries(sbx-keygen PRIVATE OpenSSL::Crypto)

add_executable(sbx-sign tools/sign_image.cpp src/crypto/signer.cpp src/crypto/hash.cpp)
target_link_libraries(sbx-sign PRIVATE OpenSSL::Crypto)

add_executable(sbx-verify tools/verify_image.cpp src/crypto/verifier.cpp src/crypto/hash.cpp)
target_link_libraries(sbx-verify PRIVATE OpenSSL::Crypto)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

# Test: Hash
add_executable(test_hash tests/test_hash.cpp src/crypto/hash.cpp)
target_link_libraries(test_hash PRIVATE GTest::gtest_main OpenSSL::Crypto)
gtest_discover_tests(test_hash)

# Test: Signature
add_executable(test_signature tests/test_signature.cpp src/crypto/signer.cpp src/crypto/verifier.cpp src/crypto/hash.cpp)
target_link_libraries(test_signature PRIVATE GTest::gtest_main OpenSSL::Crypto)
gtest_discover_tests(test_signature)

# Test: Manifest
add_executable(test_manifest tests/test_manifest.cpp src/boot/manifest.cpp src/boot/boot_component.cpp)
target_link_libraries(test_manifest PRIVATE GTest::gtest_main)
gtest_discover_tests(test_manifest)

# Test: Measurement
add_executable(test_measurement tests/test_measurement.cpp src/measurement/measurement_engine.cpp src/measurement/measurement_log.cpp src/measurement/measurement.cpp src/crypto/hash.cpp src/tpm/tpm_manager.cpp src/tpm/pcr_manager.cpp)
target_link_libraries(test_measurement PRIVATE GTest::gtest_main OpenSSL::Crypto)
gtest_discover_tests(test_measurement)

# Test: Trust Manager
add_executable(test_trust_manager tests/test_trust_manager.cpp src/trust/trust_manager.cpp src/trust/trust_policy.cpp src/measurement/measurement_log.cpp src/measurement/measurement.cpp)
target_link_libraries(test_trust_manager PRIVATE GTest::gtest_main)
gtest_discover_tests(test_trust_manager)
