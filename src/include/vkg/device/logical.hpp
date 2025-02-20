#pragma once

#include <array>
#include <utility>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vkg/device/physical.hpp"
#include "vkg/helper/defines.hpp"
#include "vkg/queuefamily.hpp"
#include "vkg/window.hpp"

namespace vkg {

class DeviceManager {
public:
    inline DeviceManager(PhyDeviceManager& phyDeviceMgr, const QueueFamilyManager& queueFamilyMgr);
    inline ~DeviceManager() noexcept;

    template <typename Self>
    [[nodiscard]] auto&& getDevice(this Self&& self) noexcept {
        return std::forward_like<Self>(self).device_;
    }

private:
    vk::Device device_;
};

DeviceManager::DeviceManager(PhyDeviceManager& phyDeviceMgr, const QueueFamilyManager& queueFamilyMgr) {
    auto& phyDevice = phyDeviceMgr.getPhysicalDevice();

    constexpr float priority = 1.0f;
    std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos;
    vk::DeviceQueueCreateInfo graphicsQueueInfo;
    graphicsQueueInfo.setQueueFamilyIndex(queueFamilyMgr.getGraphicsQFamilyIndex());
    graphicsQueueInfo.setQueueCount(1);
    graphicsQueueInfo.setQueuePriorities(priority);
    deviceQueueInfos.push_back(graphicsQueueInfo);

    if (!queueFamilyMgr.sameQFamily()) {
        vk::DeviceQueueCreateInfo presentQueueInfo;
        presentQueueInfo.setQueueFamilyIndex(queueFamilyMgr.getPresentQFamilyIndex());
        presentQueueInfo.setQueueCount(1);
        presentQueueInfo.setQueuePriorities(priority);
        deviceQueueInfos.push_back(presentQueueInfo);
    }

    vk::DeviceCreateInfo deviceInfo;
    deviceInfo.setQueueCreateInfos(deviceQueueInfos);

    constexpr std::array<const char*, 1> exts{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    deviceInfo.setPpEnabledExtensionNames(exts.data());
    deviceInfo.setEnabledExtensionCount(exts.size());

    device_ = phyDevice.createDevice(deviceInfo);
}

DeviceManager::~DeviceManager() noexcept { device_.destroy(); }

}  // namespace vkg
