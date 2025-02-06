#pragma once

#include <vulkan/vulkan.hpp>

#include "vkg/device/logical.hpp"

namespace vkg {

class CommandPoolManager {
public:
    inline CommandPoolManager(const DeviceManager& deviceMgr);
    inline ~CommandPoolManager() noexcept;

    template <typename Self>
    [[nodiscard]] auto&& getCommandPool(this Self& self) noexcept {
        return std::forward_like<Self>(self).commandPool_;
    }

private:
    const DeviceManager& deviceMgr_;  // FIXME: UAF
    vk::CommandPool commandPool_;
};

CommandPoolManager::CommandPoolManager(const DeviceManager& deviceMgr) : deviceMgr_(deviceMgr) {
    vk::CommandPoolCreateInfo commandPoolInfo;
    commandPoolInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

    const auto& device = deviceMgr.getDevice();
    commandPool_ = device.createCommandPool(commandPoolInfo);
}

CommandPoolManager::~CommandPoolManager() noexcept {
    const auto& device = deviceMgr_.getDevice();
    device.destroyCommandPool(commandPool_);
}

}  // namespace vkg
