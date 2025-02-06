#pragma once

#include <vulkan/vulkan.h>

#include "vkg/device/logical.hpp"
#include "vkg/queuefamily.hpp"

namespace vkg {

class QueueManager {
public:
    inline QueueManager(const DeviceManager& deviceMgr, const QueueFamilyManager& queueFamilyMgr);

    template <typename Self>
    [[nodiscard]] auto&& getGraphicsQueue(this Self& self) noexcept {
        return std::forward_like<Self>(self).graphicsQueue_;
    }

    template <typename Self>
    [[nodiscard]] auto&& getPresentQueue(this Self& self) noexcept {
        return std::forward_like<Self>(self).presentQueue_;
    }

private:
    vk::Queue graphicsQueue_;
    vk::Queue presentQueue_;
};

QueueManager::QueueManager(const DeviceManager& deviceMgr, const QueueFamilyManager& queueFamilyMgr) {
    const auto& device = deviceMgr.getDevice();
    graphicsQueue_ = device.getQueue(queueFamilyMgr.getGraphicsQFamilyIndex(), 0);
    presentQueue_ = device.getQueue(queueFamilyMgr.getPresentQFamilyIndex(), 0);
}

}  // namespace vkg
