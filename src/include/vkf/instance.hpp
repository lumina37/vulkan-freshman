#pragma once

#include <print>

#include <vulkan/vulkan.hpp>

#include "vkf/helper/defines.hpp"
#include "vkf/helper/env.hpp"

namespace vkf {

class InstanceManager
{
public:
    InstanceManager();
    ~InstanceManager();

    inline vk::Instance& getInstance() { return instance_; }
    inline vk::PhysicalDevice bestPhysicalDevice();

private:
    vk::Instance instance_;
};

InstanceManager::InstanceManager()
{
    constexpr bool ENABLE_VALIDATION_LAYER = ENABLE_DEBUG;

    vk::ApplicationInfo appInfo{"vk-freshman", VK_API_VERSION_1_3};
    vk::InstanceCreateInfo instInfo{{}, &appInfo};

    if constexpr (ENABLE_VALIDATION_LAYER) {
        if (hasValidationLayer()) {
            instInfo.setPEnabledLayerNames({VALIDATION_LAYER_NAME});
        }
    }

    instance_ = vk::createInstance(instInfo);
};

InstanceManager::~InstanceManager()
{
    // instance_.destroy();
}

vk::PhysicalDevice InstanceManager::bestPhysicalDevice()
{
    const auto& phyDevices = instance_.enumeratePhysicalDevices();
    const auto& candiPhyDevice = phyDevices[0];
    if constexpr (ENABLE_DEBUG) {
        std::print("Picked Physical Device: {}", candiPhyDevice.getProperties().deviceName.data());
    }

    return candiPhyDevice;
}

} // namespace vkf
