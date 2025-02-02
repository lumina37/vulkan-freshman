#pragma once

#include <array>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vkf/device/physical.hpp"
#include "vkf/helper/defines.hpp"
#include "vkf/queuefamily.hpp"
#include "vkf/window.hpp"

namespace vkf {

class DeviceManager {
public:
    inline DeviceManager(const PhyDeviceManager& phyDeviceMgr, const QueueFamilyManager& queueFamilyMgr);
    inline ~DeviceManager() noexcept;

    [[nodiscard]] inline vk::Device& getDevice() noexcept { return device_; }
    [[nodiscard]] inline const vk::Device& getDevice() const noexcept { return device_; }

private:
    vk::Device device_;
};

DeviceManager::DeviceManager(const PhyDeviceManager& phyDeviceMgr, const QueueFamilyManager& queueFamilyMgr) {
    const auto& phyDevice = phyDeviceMgr.getPhysicalDevice();

    const float priority = 1.0f;
    std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos;
    vk::DeviceQueueCreateInfo graphicsQueueInfo;
    graphicsQueueInfo.setQueuePriorities(priority);
    graphicsQueueInfo.setQueueFamilyIndex(queueFamilyMgr.getGraphicsQFamilyIndex());
    graphicsQueueInfo.setQueueCount(1);
    deviceQueueInfos.push_back(graphicsQueueInfo);

    if (!queueFamilyMgr.sameQFamily()) {
        vk::DeviceQueueCreateInfo presentQueueInfo;
        presentQueueInfo.setQueuePriorities(priority);
        presentQueueInfo.setQueueFamilyIndex(queueFamilyMgr.getPresentQFamilyIndex());
        presentQueueInfo.setQueueCount(1);
        deviceQueueInfos.push_back(presentQueueInfo);
    }

    vk::DeviceCreateInfo deviceInfo;
    deviceInfo.setQueueCreateInfos(deviceQueueInfos);

    std::array exts{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    deviceInfo.setPEnabledExtensionNames(exts);

    device_ = phyDevice.createDevice(deviceInfo);
}

DeviceManager::~DeviceManager() noexcept { device_.destroy(); }

}  // namespace vkf
