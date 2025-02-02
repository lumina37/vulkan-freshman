#pragma once

#include <filesystem>

#include <vulkan/vulkan.hpp>

#include "vkf/device/logical.hpp"
#include "vkf/helper/readfile.hpp"

namespace vkf {

namespace fs = std::filesystem;

class ShaderManager {
public:
    inline ShaderManager(const DeviceManager& deviceMgr, const fs::path& path);
    inline ~ShaderManager() noexcept;

    template <class Self>
    [[nodiscard]] inline auto&& getShaderModule(this Self& self) noexcept {
        return std::forward_like<Self>(self).shader_;
    }

private:
    const DeviceManager& deviceMgr_;  // FIXME: UAF
    vk::ShaderModule shader_;
};

ShaderManager::ShaderManager(const DeviceManager& deviceMgr, const fs::path& path) : deviceMgr_(deviceMgr) {
    const auto& code = readFile(path);

    vk::ShaderModuleCreateInfo shaderInfo;
    shaderInfo.setPCode((uint32_t*)code.data());
    shaderInfo.setCodeSize(code.size());

    const auto& device = deviceMgr.getDevice();
    shader_ = device.createShaderModule(shaderInfo);
}

ShaderManager::~ShaderManager() noexcept {
    const auto& device = deviceMgr_.getDevice();
    device.destroyShaderModule(shader_);
}

}  // namespace vkf
