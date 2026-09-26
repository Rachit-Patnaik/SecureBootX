cmake_minimum_required(VERSION 3.14)
project(SecureBootX VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/securebootx)

set(CORE_SOURCES
    src/logger.cpp
    src/firmware.cpp
    src/crypto_engine.cpp
    src/trust_framework.cpp
    src/verified_boot.cpp
    src/measured_boot.cpp
)

add_library(securebootx_core STATIC ${CORE_SOURCES})

add_executable(securebootx src/main.cpp)
target_link_libraries(securebootx PRIVATE securebootx_core)

enable_testing()

add_executable(crypto_test tests/crypto_test.cpp)
target_link_libraries(crypto_test PRIVATE securebootx_core)
add_test(NAME crypto_test COMMAND crypto_test)

add_executable(verified_boot_test tests/verified_boot_test.cpp)
target_link_libraries(verified_boot_test PRIVATE securebootx_core)
add_test(NAME verified_boot_test COMMAND verified_boot_test)

add_executable(measured_boot_test tests/measured_boot_test.cpp)
target_link_libraries(measured_boot_test PRIVATE securebootx_core)
add_test(NAME measured_boot_test COMMAND measured_boot_test)
