#include "measurement/measurement_log.hpp"
#include <fstream>
#include <iostream>

namespace securebootx {

void MeasurementLog::addEntry(const Measurement& entry) {
    m_entries.push_back(entry);
}

void MeasurementLog::saveToFile(const std::filesystem::path& path) const {
    std::ofstream file(path);
    if (!file) {
        std::cerr << "[MeasurementLog] Error: Could not open " << path << " for writing" << std::endl;
        return;
    }

    file << "[\n";
    for (size_t i = 0; i < m_entries.size(); ++i) {
        file << m_entries[i].toJson() << (i == m_entries.size() - 1 ? "" : ",") << "\n";
    }
    file << "]\n";
}

void MeasurementLog::clear() {
    m_entries.clear();
}

} // namespace securebootx
