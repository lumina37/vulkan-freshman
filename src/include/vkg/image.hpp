#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vkg/device/logical.hpp"
#include "vkg/renderpass.hpp"
#include "vkg/swapchain.hpp"

namespace vkg {

class ImageManager {
public:
    inline ImageManager(DeviceManager& deviceMgr, const vk::Extent2D extent, const SwapChainManager& swapchainMgr,
                        const RenderPassManager& renderPassMgr);
    inline ~ImageManager() noexcept;

    template <typename Self>
    [[nodiscard]] auto&& getFrameBuffers(this Self&& self) noexcept {
        return std::forward_like<Self>(self).frameBuffers_;
    }

private:
    DeviceManager& deviceMgr_;  // FIXME: UAF
    std::vector<vk::Image> images_;
    std::vector<vk::ImageView> imageViews_;
    std::vector<vk::Framebuffer> frameBuffers_;
};

ImageManager::ImageManager(DeviceManager& deviceMgr, const vk::Extent2D extent, const SwapChainManager& swapchainMgr,
                           const RenderPassManager& renderPassMgr)
    : deviceMgr_(deviceMgr) {
    auto& device = deviceMgr.getDevice();
    const auto& swapchain = swapchainMgr.getSwapchain();

    images_ = device.getSwapchainImagesKHR(swapchain);

    imageViews_.reserve(images_.size());
    frameBuffers_.reserve(images_.size());
    for (auto& image : images_) {
        vk::ImageSubresourceRange subResRange;
        subResRange.setAspectMask(vk::ImageAspectFlagBits::eColor);
        subResRange.setLevelCount(1);
        subResRange.setLayerCount(1);

        vk::ImageViewCreateInfo viewInfo;
        viewInfo.setImage(image);
        viewInfo.setViewType(vk::ImageViewType::e2D);
        viewInfo.setFormat(SwapChainManager::IMAGE_FORMAT);
        viewInfo.setSubresourceRange(subResRange);
        auto imageView = device.createImageView(viewInfo);
        imageViews_.push_back(imageView);

        vk::FramebufferCreateInfo frameBufferInfo;
        frameBufferInfo.setRenderPass(renderPassMgr.getRenderPass());
        frameBufferInfo.setAttachments(imageView);
        frameBufferInfo.setWidth(extent.width);
        frameBufferInfo.setHeight(extent.height);
        frameBufferInfo.setLayers(1);

        auto frameBuffer = device.createFramebuffer(frameBufferInfo);
        frameBuffers_.push_back(frameBuffer);
    }
}

ImageManager::~ImageManager() noexcept {
    auto& device = deviceMgr_.getDevice();
    for (auto& frameBuffer : frameBuffers_) {
        device.destroyFramebuffer(frameBuffer);
    }
    for (auto& imageView : imageViews_) {
        device.destroyImageView(imageView);
    }
}

}  // namespace vkg
