#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vkg/command/pool.hpp"
#include "vkg/device/logical.hpp"

namespace vkg {

class CommandBufferManager {
public:
    inline CommandBufferManager(const DeviceManager& deviceMgr, const CommandPoolManager& commandPoolMgr);
    inline ~CommandBufferManager() noexcept;

    template <typename Self>
    [[nodiscard]] auto&& getCommandBuffers(this Self& self) noexcept {
        return std::forward_like<Self>(self).commandBuffers_;
    }

private:
    const DeviceManager& deviceMgr_;            // FIXME: UAF
    const CommandPoolManager& commandPoolMgr_;  // FIXME: UAF
    std::vector<vk::CommandBuffer> commandBuffers_;
};

CommandBufferManager::CommandBufferManager(const DeviceManager& deviceMgr, const CommandPoolManager& commandPoolMgr)
    : deviceMgr_(deviceMgr), commandPoolMgr_(commandPoolMgr) {
    const auto& device = deviceMgr.getDevice();
    const auto& commandPool = commandPoolMgr.getCommandPool();

    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.setCommandPool(commandPool);
    allocInfo.setCommandBufferCount(1);
    allocInfo.setLevel(vk::CommandBufferLevel::ePrimary);

    commandBuffers_ = device.allocateCommandBuffers(allocInfo);
}

CommandBufferManager::~CommandBufferManager() noexcept {
    const auto& device = deviceMgr_.getDevice();
    const auto& commandPool = commandPoolMgr_.getCommandPool();
    device.freeCommandBuffers(commandPool, commandBuffers_);
}

}  // namespace vkg
