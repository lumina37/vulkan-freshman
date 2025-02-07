#pragma once

#include <cstdint>
#include <filesystem>
#include <utility>

#include <vulkan/vulkan.hpp>

#include "vkg/device/logical.hpp"
#include "vkg/helper/readfile.hpp"

namespace vkg {

namespace fs = std::filesystem;

class ShaderManager {
public:
    inline ShaderManager(const DeviceManager& deviceMgr, const fs::path& path);
    inline ~ShaderManager() noexcept;

    template <typename Self>
    [[nodiscard]] auto&& getShaderModule(this Self& self) noexcept {
        return std::forward_like<Self>(self).shader_;
    }

private:
    const DeviceManager& deviceMgr_;  // FIXME: UAF
    vk::ShaderModule shader_;
};

ShaderManager::ShaderManager(const DeviceManager& deviceMgr, const fs::path& path) : deviceMgr_(deviceMgr) {
    const auto& code = readFile(path);

    vk::ShaderModuleCreateInfo shaderInfo;
    shaderInfo.setCodeSize(code.size());
    shaderInfo.setPCode((uint32_t*)code.data());

    const auto& device = deviceMgr.getDevice();
    shader_ = device.createShaderModule(shaderInfo);
}

ShaderManager::~ShaderManager() noexcept {
    const auto& device = deviceMgr_.getDevice();
    device.destroyShaderModule(shader_);
}

}  // namespace vkg
