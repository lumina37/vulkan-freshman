#pragma once

#include <utility>

#include <vulkan/vulkan.hpp>

#include "vkg/device/logical.hpp"
#include "vkg/swapchain.hpp"

namespace vkg {

class RenderPassManager {
public:
    inline RenderPassManager(const DeviceManager& deviceMgr);
    inline ~RenderPassManager() noexcept;

    template <class Self>
    [[nodiscard]] auto&& getRenderPass(this Self& self) noexcept {
        return std::forward_like<Self>(self).renderPass_;
    }

private:
    const DeviceManager& deviceMgr_;  // FIXME: UAF
    vk::RenderPass renderPass_;
};

RenderPassManager::RenderPassManager(const DeviceManager& deviceMgr) : deviceMgr_(deviceMgr) {
    vk::RenderPassCreateInfo renderPassInfo;

    vk::AttachmentDescription attachDesc;
    attachDesc.setFormat(SwapChainManager::IMAGE_FORMAT);
    attachDesc.setInitialLayout(vk::ImageLayout::eUndefined);
    attachDesc.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);
    attachDesc.setLoadOp(vk::AttachmentLoadOp::eClear);
    attachDesc.setStoreOp(vk::AttachmentStoreOp::eStore);
    attachDesc.setSamples(vk::SampleCountFlagBits::e1);
    // TODO: Stencil OP
    renderPassInfo.setAttachments(attachDesc);

    vk::AttachmentReference attachRef;
    attachRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);
    attachRef.setAttachment(0);
    vk::SubpassDescription subpassDesc;
    subpassDesc.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpassDesc.setColorAttachments(attachRef);
    renderPassInfo.setSubpasses(subpassDesc);

    vk::SubpassDependency subpassDep;
    subpassDep.setSrcSubpass(vk::SubpassExternal);
    subpassDep.setDstSubpass(0);
    subpassDep.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);
    subpassDep.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    subpassDep.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    renderPassInfo.setDependencies(subpassDep);

    const auto& device = deviceMgr.getDevice();
    renderPass_ = device.createRenderPass(renderPassInfo);
}

RenderPassManager::~RenderPassManager() noexcept {
    const auto& device = deviceMgr_.getDevice();
    device.destroyRenderPass(renderPass_);
}

}  // namespace vkg
