#pragma once

#include "measurement/measurement.hpp"
#include <vector>
#include <filesystem>
#include <string>

namespace securebootx {

class MeasurementLog {
public:
    /**
     * @brief Adds a measurement to the log.
     */
    void addEntry(const Measurement& entry);

    /**
     * @brief Saves the log to a JSON file.
     */
    void saveToFile(const std::filesystem::path& path) const;

    /**
     * @brief Clears the log for a new boot session.
     */
    void clear();

    const std::vector<Measurement>& getEntries() const { return m_entries; }

private:
    std::vector<Measurement> m_entries;
};

} // namespace securebootx
