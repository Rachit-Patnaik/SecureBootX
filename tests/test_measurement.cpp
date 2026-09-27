#include <gtest/gtest.h>
#include "measurement/measurement_engine.hpp"
#include "measurement/measurement_log.hpp"
#include "boot/boot_component.hpp"
#include <memory>
#include <fstream>
#include <filesystem>

using namespace securebootx;

TEST(MeasurementTest, PcrExtensionLogic) {
    auto log = std::make_shared<MeasurementLog>();
    MeasurementEngine engine(log);

    BootComponent comp;
    comp.name = "TestComp";
    comp.path = "measure_test.bin";
    comp.pcrIndex = 4;

    std::ofstream ofs(comp.path, std::ios::binary);
    ofs << "Measurement data";
    ofs.close();

    std::string pcrBefore = engine.getPcrValue(4);
    engine.measure(comp);
    std::string pcrAfter = engine.getPcrValue(4);

    EXPECT_NE(pcrBefore, pcrAfter);
    EXPECT_EQ(log->getEntries().size(), 1);
    EXPECT_TRUE(log->getEntries()[0].success);

    std::filesystem::remove(comp.path);
}
