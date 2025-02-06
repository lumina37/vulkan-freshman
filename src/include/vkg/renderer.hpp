#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <print>

#include <vulkan/vulkan.hpp>

#include "vkg/command/buffer.hpp"
#include "vkg/device/logical.hpp"
#include "vkg/helper/defines.hpp"
#include "vkg/pipeline.hpp"
#include "vkg/queue.hpp"
#include "vkg/renderpass.hpp"
#include "vkg/swapchain.hpp"

namespace vkg {

class Renderer {
public:
    inline Renderer(const DeviceManager& deviceMgr, const SwapChainManager& swapchainMgr,
                    const CommandBufferManager& commandBufferMgr, const PipelineManager& pipelineMgr,
                    const RenderPassManager& renderPassMgr, const ImageManager& imageMgr, const vk::Extent2D& extent,
                    const QueueManager& queueMgr);
    inline ~Renderer() noexcept;

    inline void render();

private:
    const DeviceManager& deviceMgr_;
    const SwapChainManager& swapchainMgr_;
    const CommandBufferManager& commandBufferMgr_;
    const PipelineManager& pipelineMgr_;
    const RenderPassManager& renderPassMgr_;
    const ImageManager& imageMgr_;
    vk::Extent2D extent_;
    const QueueManager& queueMgr_;
    vk::Fence commandCompleteFence_;
    vk::Semaphore imageAvailableSem_;
};

Renderer::Renderer(const DeviceManager& deviceMgr, const SwapChainManager& swapchainMgr,
                   const CommandBufferManager& commandBufferMgr, const PipelineManager& pipelineMgr,
                   const RenderPassManager& renderPassMgr, const ImageManager& imageMgr, const vk::Extent2D& extent,
                   const QueueManager& queueMgr)
    : deviceMgr_(deviceMgr),
      swapchainMgr_(swapchainMgr),
      commandBufferMgr_(commandBufferMgr),
      pipelineMgr_(pipelineMgr),
      renderPassMgr_(renderPassMgr),
      imageMgr_(imageMgr),
      extent_(extent),
      queueMgr_(queueMgr) {
    vk::FenceCreateInfo fenceInfo;
    // fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);
    const auto& device = deviceMgr.getDevice();
    commandCompleteFence_ = device.createFence(fenceInfo);

    vk::SemaphoreCreateInfo semaphoreInfo;
    imageAvailableSem_ = device.createSemaphore(semaphoreInfo);
}

inline Renderer::~Renderer() noexcept {
    const auto& device = deviceMgr_.getDevice();
    device.destroyFence(commandCompleteFence_);
    device.destroySemaphore(imageAvailableSem_);
}

void Renderer::render() {
    const auto& device = deviceMgr_.getDevice();
    const auto& swapchain = swapchainMgr_.getSwapchain();

    const auto& nextImageIndexResult =
        device.acquireNextImageKHR(swapchain, std::numeric_limits<uint64_t>::max(), imageAvailableSem_);
    if constexpr (ENABLE_DEBUG) {
        if (nextImageIndexResult.result != vk::Result::eSuccess) {
            std::println(std::cerr, "Failed to acquire next swapchain image!");
        }
    }
    auto nextImageIndex = nextImageIndexResult.value;

    const auto& cmdBuf = commandBufferMgr_.getCommandBuffers()[0];
    cmdBuf.reset();

    // Begin Command Buffer
    vk::CommandBufferBeginInfo cmdBufBeginInfo;
    cmdBufBeginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmdBuf.begin(cmdBufBeginInfo);

    cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineMgr_.getPipeline());

    // Begin Render Pass
    vk::RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.setRenderPass(renderPassMgr_.getRenderPass());

    const auto& frameBuffers = imageMgr_.getFrameBuffers();
    renderPassBeginInfo.setFramebuffer(frameBuffers[nextImageIndex]);

    vk::Rect2D scissor;
    scissor.setOffset({0, 0});
    scissor.setExtent(extent_);
    renderPassBeginInfo.setRenderArea(scissor);

    vk::ClearValue clearValue;
    clearValue.setColor({0.1f, 0.1f, 0.1f, 1.0f});
    renderPassBeginInfo.setClearValues(clearValue);

    cmdBuf.beginRenderPass(renderPassBeginInfo, {});
    cmdBuf.draw(3, 1, 0, 0);
    cmdBuf.endRenderPass();

    cmdBuf.end();

    // Submit
    vk::SubmitInfo submitInfo;
    submitInfo.setCommandBuffers(cmdBuf);
    submitInfo.setWaitSemaphores(imageAvailableSem_);
    constexpr vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    submitInfo.setWaitDstStageMask(waitStage);

    const auto& graphicsQueue = queueMgr_.getGraphicsQueue();
    graphicsQueue.submit(submitInfo, commandCompleteFence_);

    const auto& waitFenceResult =
        device.waitForFences(commandCompleteFence_, true, std::numeric_limits<uint64_t>::max());
    if constexpr (ENABLE_DEBUG) {
        if (waitFenceResult != vk::Result::eSuccess) {
            std::println(std::cerr, "Command fence timeout!");
        }
    }
    device.resetFences(commandCompleteFence_);

    // Present
    vk::PresentInfoKHR presentInfo;
    presentInfo.setImageIndices(nextImageIndex);
    presentInfo.setSwapchains(swapchain);

    const auto& presentQueue = queueMgr_.getPresentQueue();
    const auto& presentResult = presentQueue.presentKHR(presentInfo);
    if constexpr (ENABLE_DEBUG) {
        if (presentResult != vk::Result::eSuccess) {
            std::println(std::cerr, "Present Failed!");
        }
    }
}

}  // namespace vkg
