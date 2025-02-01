#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vkf/device/logical.hpp"
#include "vkf/queuefamily.hpp"
#include "vkf/surface.hpp"
#include "vkf/window.hpp"

namespace vkf {

class SwapChainManager {
public:
    inline SwapChainManager(const DeviceManager& deviceMgr, const SurfaceManager& surfaceMgr,
                            const QueueFamilyManager& queuefamilyMgr, const vk::Extent2D& extent);
    inline ~SwapChainManager() noexcept;

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
    swapchainInfo.setImageFormat(vk::Format::eB8G8R8A8Unorm);             // TODO: auto-select
    swapchainInfo.setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear);  // TODO: auto-select
    swapchainInfo.setImageArrayLayers(1);
    swapchainInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
    swapchainInfo.setMinImageCount(3);
    swapchainInfo.setClipped(true);
    swapchainInfo.setPresentMode(vk::PresentModeKHR::eFifo);

    if (queuefamilyMgr.sameQFamily()) {
        swapchainInfo.setImageSharingMode(vk::SharingMode::eExclusive);
    } else {
        swapchainInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
        swapchainInfo.setQueueFamilyIndexCount(2);
        std::vector<uint32_t> queuefamilyIndices{queuefamilyMgr.getGraphicsQFamilyIndex(),
                                                 queuefamilyMgr.getPresentQFamilyIndex()};
        swapchainInfo.setQueueFamilyIndices(queuefamilyIndices);
    }

    const auto& device = deviceMgr.getDevice();
    swapchain_ = device.createSwapchainKHR(swapchainInfo);
}

SwapChainManager::~SwapChainManager() noexcept { deviceMgr_.getDevice().destroy(swapchain_); }

}  // namespace vkf
