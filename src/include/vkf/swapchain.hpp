#pragma once

#include <array>
#include <utility>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vkf/device/logical.hpp"
#include "vkf/queuefamily.hpp"
#include "vkf/surface.hpp"
#include "vkf/window.hpp"

namespace vkf {

class SwapChainManager {
public:
    static constexpr vk::Format IMAGE_FORMAT = vk::Format::eB8G8R8A8Unorm;  // TODO: auto-select

    inline SwapChainManager(const DeviceManager& deviceMgr, const SurfaceManager& surfaceMgr,
                            const QueueFamilyManager& queuefamilyMgr, const vk::Extent2D& extent);
    inline ~SwapChainManager() noexcept;

    template <class Self>
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
    swapchainInfo.setImageExtent(extent);
    swapchainInfo.setImageFormat(IMAGE_FORMAT);
    swapchainInfo.setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear);  // TODO: auto-select
    swapchainInfo.setImageArrayLayers(1);
    swapchainInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
    swapchainInfo.setMinImageCount(2);
    swapchainInfo.setClipped(true);
    swapchainInfo.setPresentMode(vk::PresentModeKHR::eFifo);

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

}  // namespace vkf
