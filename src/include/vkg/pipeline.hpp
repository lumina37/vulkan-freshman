#pragma once

#include <array>
#include <iostream>
#include <print>

#include <vulkan/vulkan.hpp>

#include "vkg/device/logical.hpp"
#include "vkg/renderpass.hpp"
#include "vkg/shader.hpp"

namespace vkg {

class PipelineManager {
public:
    inline PipelineManager(DeviceManager& deviceMgr, const vk::Extent2D extent, const ShaderManager& vertShaderMgr,
                           const ShaderManager& fragShaderMgr, const RenderPassManager& renderPassMgr);
    inline ~PipelineManager() noexcept;

    template <typename Self>
    [[nodiscard]] auto&& getPipeline(this Self&& self) noexcept {
        return std::forward_like<Self>(self).pipeline_;
    }

private:
    DeviceManager& deviceMgr_;           // FIXME: UAF
    vk::PipelineLayout pipelineLayout_;  // TODO: Extract components
    vk::Pipeline pipeline_;
};

PipelineManager::PipelineManager(DeviceManager& deviceMgr, const vk::Extent2D extent,
                                 const ShaderManager& vertShaderMgr, const ShaderManager& fragShaderMgr,
                                 const RenderPassManager& renderPassMgr)
    : deviceMgr_(deviceMgr) {
    vk::GraphicsPipelineCreateInfo pipelineInfo;

    // Vertex Inputs
    vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo;
    pipelineInfo.setPVertexInputState(&vertexInputStateInfo);

    // Input ASM
    vk::PipelineInputAssemblyStateCreateInfo inputAsmStateInfo;
    inputAsmStateInfo.setTopology(vk::PrimitiveTopology::eTriangleList);
    pipelineInfo.setPInputAssemblyState(&inputAsmStateInfo);

    // Shaders
    vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
    vertShaderStageInfo.setStage(vk::ShaderStageFlagBits::eVertex);
    vertShaderStageInfo.setModule(vertShaderMgr.getShaderModule());
    vertShaderStageInfo.setPName("main");
    vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
    fragShaderStageInfo.setStage(vk::ShaderStageFlagBits::eFragment);
    fragShaderStageInfo.setModule(fragShaderMgr.getShaderModule());
    fragShaderStageInfo.setPName("main");
    const std::array shaderStageInfos{vertShaderStageInfo, fragShaderStageInfo};
    pipelineInfo.setStages(shaderStageInfos);

    // Viewport
    vk::Viewport viewport;
    viewport.setWidth((float)extent.width);
    viewport.setHeight((float)extent.height);
    vk::Rect2D scissor;
    scissor.setOffset({0, 0});
    scissor.setExtent(extent);
    vk::PipelineViewportStateCreateInfo viewportStateInfo;
    viewportStateInfo.setViewports(viewport);
    viewportStateInfo.setScissors(scissor);
    pipelineInfo.setPViewportState(&viewportStateInfo);

    // Rasterization
    vk::PipelineRasterizationStateCreateInfo rasterStateInfo;
    rasterStateInfo.setPolygonMode(vk::PolygonMode::eFill);
    rasterStateInfo.setCullMode(vk::CullModeFlagBits::eBack);
    rasterStateInfo.setFrontFace(vk::FrontFace::eCounterClockwise);
    rasterStateInfo.setLineWidth(1);
    pipelineInfo.setPRasterizationState(&rasterStateInfo);

    // Multi Sampling
    vk::PipelineMultisampleStateCreateInfo multisampleInfo;
    multisampleInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);
    pipelineInfo.setPMultisampleState(&multisampleInfo);

    // Color Blending
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                           vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
    vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
    colorBlendInfo.setLogicOpEnable(false);
    colorBlendInfo.setAttachments(colorBlendAttachment);
    pipelineInfo.setPColorBlendState(&colorBlendInfo);

    // Render Pass
    const auto& renderPass = renderPassMgr.getRenderPass();
    pipelineInfo.setRenderPass(renderPass);

    // Pipeline Layout
    auto& device = deviceMgr.getDevice();
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayout_ = device.createPipelineLayout(pipelineLayoutInfo);
    pipelineInfo.setLayout(pipelineLayout_);

    // Create Pipeline
    auto pipelineResult = device.createGraphicsPipeline(nullptr, pipelineInfo);
    if constexpr (ENABLE_DEBUG) {
        if (pipelineResult.result != vk::Result::eSuccess) {
            std::println(std::cerr, "Failed to create graphics pipeline. err: {}", (int)pipelineResult.result);
        }
    }
    pipeline_ = pipelineResult.value;
}

PipelineManager::~PipelineManager() noexcept {
    auto& device = deviceMgr_.getDevice();
    device.destroyPipelineLayout(pipelineLayout_);
    device.destroyPipeline(pipeline_);
}

}  // namespace vkg
