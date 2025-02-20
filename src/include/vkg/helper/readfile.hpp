#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <vector>

#include "vkg/helper/defines.hpp"

namespace vkg {

namespace fs = std::filesystem;

static inline std::vector<std::byte> readFile(const fs::path& path) {
    std::ifstream file{path, std::ios::ate | std::ios::binary};

    if constexpr (ENABLE_DEBUG) {
        if (!file.is_open()) {
            std::println(std::cerr, "Cannot open file: {}", path.string());
        }
    }

    const std::streamsize fileSize = file.tellg();
    std::vector<std::byte> buffer(fileSize);

    file.seekg(0);
    file.read((char*)buffer.data(), fileSize);

    return buffer;
}

}  // namespace vkg
