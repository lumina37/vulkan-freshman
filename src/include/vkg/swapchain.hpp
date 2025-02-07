#pragma once

#include <array>
#include <utility>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vkg/device/logical.hpp"
#include "vkg/queuefamily.hpp"
#include "vkg/surface.hpp"
#include "vkg/window.hpp"

namespace vkg {

class SwapChainManager {
public:
    static constexpr vk::Format IMAGE_FORMAT = vk::Format::eB8G8R8A8Unorm;  // TODO: auto-select

    inline SwapChainManager(const DeviceManager& deviceMgr, const SurfaceManager& surfaceMgr,
                            const QueueFamilyManager& queuefamilyMgr, const vk::Extent2D& extent);
    inline ~SwapChainManager() noexcept;

    template <typename Self>
    [[nodiscard]] auto&& getSwapchain(this Self& self) noexcept {
        return std::forward_like<Self>(self).swapchain_;
    }

private:
    const DeviceManager& deviceMgr_;  // FIXME: UAF
    vk::SwapchainKHR swapchain_;
};

SwapChainManager::SwapChainManager(const DeviceManager& deviceMgr, const SurfaceManager& surfaceMgr,
                                   const QueueFamilyManager& queuefamilyMgr, const vk::Extent2D& extent)
    : deviceMgr_(deviceMgr) {
    vk::SwapchainCreateInfoKHR swapchainInfo;
    swapchainInfo.setSurface(surfaceMgr.getSurface());
    swapchainInfo.setMinImageCount(2);
    swapchainInfo.setImageFormat(IMAGE_FORMAT);
    swapchainInfo.setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear);  // TODO: auto-select
    swapchainInfo.setImageExtent(extent);
    swapchainInfo.setImageArrayLayers(1);
    swapchainInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
    swapchainInfo.setPresentMode(vk::PresentModeKHR::eFifo);
    swapchainInfo.setClipped(true);

    if (queuefamilyMgr.sameQFamily()) {
        swapchainInfo.setImageSharingMode(vk::SharingMode::eExclusive);
    } else {
        swapchainInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
        swapchainInfo.setQueueFamilyIndexCount(2);
        const std::array queuefamilyIndices{queuefamilyMgr.getGraphicsQFamilyIndex(),
                                            queuefamilyMgr.getPresentQFamilyIndex()};
        swapchainInfo.setQueueFamilyIndices(queuefamilyIndices);
    }

    const auto& device = deviceMgr.getDevice();
    swapchain_ = device.createSwapchainKHR(swapchainInfo);
}

SwapChainManager::~SwapChainManager() noexcept {
    const auto& device = deviceMgr_.getDevice();
    device.destroySwapchainKHR(swapchain_);
}

}  // namespace vkg
