#pragma once

#include <print>
#include <utility>

#include <vulkan/vulkan.hpp>

#include "vkg/helper/defines.hpp"
#include "vkg/instance.hpp"

namespace vkg {

class PhyDeviceManager {
public:
    inline PhyDeviceManager(const InstanceManager& instMgr);

    template <class Self>
    [[nodiscard]] auto&& getPhysicalDevice(this Self& self) noexcept {
        return std::forward_like<Self>(self).physicalDevice_;
    }

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

}  // namespace vkg
