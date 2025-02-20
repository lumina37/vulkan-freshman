#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <print>

#include <vulkan/vulkan.hpp>

#include "vkg/command/pool.hpp"
#include "vkg/device/logical.hpp"
#include "vkg/helper/defines.hpp"
#include "vkg/pipeline.hpp"
#include "vkg/queue.hpp"
#include "vkg/renderpass.hpp"
#include "vkg/swapchain.hpp"

namespace vkg {

class Renderer {
public:
    inline Renderer(DeviceManager& deviceMgr, SwapChainManager& swapchainMgr, CommandPoolManager& commandPoolMgr,
                    PipelineManager& pipelineMgr, const RenderPassManager& renderPassMgr, ImageManager& imageMgr,
                    const vk::Extent2D extent, QueueManager& queueMgr);
    inline ~Renderer() noexcept;

    inline void render();

private:
    // FIXME: lots of UAF
    DeviceManager& deviceMgr_;
    SwapChainManager& swapchainMgr_;
    CommandPoolManager& commandPoolMgr_;
    PipelineManager& pipelineMgr_;
    const RenderPassManager& renderPassMgr_;
    ImageManager& imageMgr_;
    vk::Extent2D extent_;
    QueueManager& queueMgr_;
    CommandBufferManager commandBufferMgr_;
    vk::Fence commandCompleteFence_;
    vk::Semaphore imageAvailableSem_;
};

Renderer::Renderer(DeviceManager& deviceMgr, SwapChainManager& swapchainMgr, CommandPoolManager& commandPoolMgr,
                   PipelineManager& pipelineMgr, const RenderPassManager& renderPassMgr, ImageManager& imageMgr,
                   const vk::Extent2D extent, QueueManager& queueMgr)
    : deviceMgr_(deviceMgr),
      swapchainMgr_(swapchainMgr),
      commandPoolMgr_(commandPoolMgr),
      pipelineMgr_(pipelineMgr),
      renderPassMgr_(renderPassMgr),
      imageMgr_(imageMgr),
      extent_(extent),
      queueMgr_(queueMgr),
      commandBufferMgr_(deviceMgr, commandPoolMgr) {
    vk::FenceCreateInfo fenceInfo;
    // fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);
    auto& device = deviceMgr.getDevice();
    commandCompleteFence_ = device.createFence(fenceInfo);

    vk::SemaphoreCreateInfo semaphoreInfo;
    imageAvailableSem_ = device.createSemaphore(semaphoreInfo);
}

inline Renderer::~Renderer() noexcept {
    auto& device = deviceMgr_.getDevice();
    device.destroyFence(commandCompleteFence_);
    device.destroySemaphore(imageAvailableSem_);
}

void Renderer::render() {
    auto& device = deviceMgr_.getDevice();
    auto& swapchain = swapchainMgr_.getSwapchain();

    const auto nextImageIndexResult =
        device.acquireNextImageKHR(swapchain, std::numeric_limits<uint64_t>::max(), imageAvailableSem_);
    if constexpr (ENABLE_DEBUG) {
        if (nextImageIndexResult.result != vk::Result::eSuccess) {
            std::println(std::cerr, "Failed to acquire next swapchain image!");
        }
    }
    auto nextImageIndex = nextImageIndexResult.value;

    auto cmdBuf = commandBufferMgr_.getCommandBuffers()[0];
    cmdBuf.reset();

    // Begin Command Buffer
    vk::CommandBufferBeginInfo cmdBufBeginInfo;
    cmdBufBeginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmdBuf.begin(cmdBufBeginInfo);

    cmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineMgr_.getPipeline());

    // Begin Render Pass
    vk::RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.setRenderPass(renderPassMgr_.getRenderPass());

    auto& frameBuffers = imageMgr_.getFrameBuffers();
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

    auto& graphicsQueue = queueMgr_.getGraphicsQueue();
    graphicsQueue.submit(submitInfo, commandCompleteFence_);

    const auto waitFenceResult =
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

    auto& presentQueue = queueMgr_.getPresentQueue();
    const auto presentResult = presentQueue.presentKHR(presentInfo);
    if constexpr (ENABLE_DEBUG) {
        if (presentResult != vk::Result::eSuccess) {
            std::println(std::cerr, "Present Failed!");
        }
    }
}

}  // namespace vkg
