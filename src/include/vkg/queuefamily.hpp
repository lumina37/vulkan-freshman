#pragma once

#include <print>
#include <ranges>

#include <vulkan/vulkan.hpp>

#include "vkg/device/physical.hpp"
#include "vkg/helper/defines.hpp"
#include "vkg/instance.hpp"
#include "vkg/surface.hpp"

namespace vkg {

namespace rgs = std::ranges;

class QueueFamilyManager {
public:
    inline QueueFamilyManager(const InstanceManager& instMgr, const PhyDeviceManager& phyDeviceMgr,
                              const SurfaceManager& surfaceMgr);

    [[nodiscard]] inline uint32_t getGraphicsQFamilyIndex() const noexcept { return graphicsQFamilyIndex_; }
    [[nodiscard]] inline uint32_t getPresentQFamilyIndex() const noexcept { return presentQFamilyIndex_; }
    [[nodiscard]] inline bool sameQFamily() const noexcept { return graphicsQFamilyIndex_ == presentQFamilyIndex_; }

private:
    uint32_t graphicsQFamilyIndex_;
    uint32_t presentQFamilyIndex_;
};

QueueFamilyManager::QueueFamilyManager(const InstanceManager& instMgr, const PhyDeviceManager& phyDeviceMgr,
                                       const SurfaceManager& surfaceMgr)
    : graphicsQFamilyIndex_(0), presentQFamilyIndex_(0) {
    const auto& physicalDevice = phyDeviceMgr.getPhysicalDevice();
    const auto& surface = surfaceMgr.getSurface();

    const auto& queueFamilyProps = physicalDevice.getQueueFamilyProperties();
    for (const auto& [idx, queueFamilyProp] : rgs::views::enumerate(queueFamilyProps)) {
        if (queueFamilyProp.queueFlags & vk::QueueFlagBits::eGraphics) {
            if (!physicalDevice.getSurfaceSupportKHR(idx, surface)) {
                continue;
            }
            graphicsQFamilyIndex_ = (uint32_t)idx;
            presentQFamilyIndex_ = (uint32_t)idx;
            if constexpr (ENABLE_DEBUG) {
                std::println("Findout a sufficient queue family: {}", graphicsQFamilyIndex_);
            }
        }
    }
}

}  // namespace vkg
