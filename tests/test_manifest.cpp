#include <gtest/gtest.h>
#include "boot/manifest.hpp"
#include "boot/boot_component.hpp"
#include <filesystem>
#include <fstream>

using namespace securebootx;

TEST(ManifestTest, LoadAndSave) {
    Manifest m;
    BootComponent comp;
    comp.name = "Test Kernel";
    comp.type = ComponentType::KERNEL;
    comp.path = "test_kernel.bin";
    comp.expectedHash = "1234567890abcdef";

    m.addComponent(comp);
    m.setPublicKeyPath("keys/public.pem");
    m.save("test_manifest.json");

    Manifest m2;
    ASSERT_TRUE(m2.load("test_manifest.json"));
    ASSERT_EQ(m2.getComponents().size(), 1);
    EXPECT_EQ(m2.getComponents()[0].name, "Test Kernel");
    EXPECT_EQ(m2.getPublicKeyPath(), "keys/public.pem");

    std::filesystem::remove("test_manifest.json");
}
