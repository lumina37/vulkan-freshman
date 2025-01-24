#pragma once

#include <print>
#include <ranges>

#include <vulkan/vulkan.hpp>

#include "vkf/instance.hpp"

namespace vkf {

namespace rgs = std::ranges;

class PhyDeviceManager {
public:
    inline PhyDeviceManager(const InstanceManager& instMgr);

    inline vk::PhysicalDevice& getPhysicalDevice() noexcept { return physicalDevice_; }
    inline const vk::PhysicalDevice& getPhysicalDevice() const noexcept { return physicalDevice_; }
    inline const uint32_t getGraphicsQueueFamilyIndex() const noexcept { return graphicsQueueFamilyIndex_; }

private:
    vk::PhysicalDevice physicalDevice_;
    uint32_t graphicsQueueFamilyIndex_;
};

PhyDeviceManager::PhyDeviceManager(const InstanceManager& instMgr) : graphicsQueueFamilyIndex_(0) {
    const auto& instance = instMgr.getInstance();
    const auto& phyDevices = instance.enumeratePhysicalDevices();
    physicalDevice_ = phyDevices[0];  // TODO: rank by features
    if constexpr (ENABLE_DEBUG) {
        std::print("Init physical device: {}\n", physicalDevice_.getProperties().deviceName.data());
    }

    const auto& queueFamilyProps = physicalDevice_.getQueueFamilyProperties();
    for (const auto& [idx, queueFamilyProp] : rgs::views::enumerate(queueFamilyProps)) {
        if (queueFamilyProp.queueFlags & vk::QueueFlagBits::eGraphics) {
            graphicsQueueFamilyIndex_ = (uint32_t)idx;
            if constexpr (ENABLE_DEBUG) {
                std::print("Findout a matched graphics queue family: {}\n", graphicsQueueFamilyIndex_);
            }
            break;
        }
    }
}

class DeviceManager {
public:
    inline DeviceManager(const PhyDeviceManager& phyDeviceMgr);
    inline ~DeviceManager();

private:
    vk::Device device_;
};

DeviceManager::DeviceManager(const PhyDeviceManager& phyDeviceMgr) {
    const auto& phyDevice = phyDeviceMgr.getPhysicalDevice();

    vk::DeviceQueueCreateInfo deviceQueueInfo;
    const float priority = 1.0f;
    deviceQueueInfo.setQueuePriorities(priority);
    deviceQueueInfo.setQueueFamilyIndex(phyDeviceMgr.getGraphicsQueueFamilyIndex());
    deviceQueueInfo.setQueueCount(1);

    vk::DeviceCreateInfo deviceInfo;
    deviceInfo.setQueueCreateInfos(deviceQueueInfo);

    device_ = phyDevice.createDevice(deviceInfo);
}

DeviceManager::~DeviceManager() { device_.destroy(); }

}  // namespace vkf
