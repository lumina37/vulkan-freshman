#pragma once

#include <print>

#include <vulkan/vulkan.hpp>

#include "vkf/helper/defines.hpp"
#include "vkf/instance.hpp"

namespace vkf {

class PhyDeviceManager {
public:
    inline PhyDeviceManager(const InstanceManager& instMgr);

    [[nodiscard]] inline vk::PhysicalDevice& getPhysicalDevice() noexcept { return physicalDevice_; }
    [[nodiscard]] inline const vk::PhysicalDevice& getPhysicalDevice() const noexcept { return physicalDevice_; }

private:
    vk::PhysicalDevice physicalDevice_;
};

PhyDeviceManager::PhyDeviceManager(const InstanceManager& instMgr) {
    const auto& instance = instMgr.getInstance();
    const auto& phyDevices = instance.enumeratePhysicalDevices();
    physicalDevice_ = phyDevices[0];  // TODO: rank by features
    if constexpr (ENABLE_DEBUG) {
        std::println("Init physical device: {}", physicalDevice_.getProperties().deviceName.data());
    }
}

}  // namespace vkf
