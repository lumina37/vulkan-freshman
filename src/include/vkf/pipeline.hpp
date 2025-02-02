#pragma once

#include <array>

#include <vulkan/vulkan.hpp>

#include "vkf/shader.hpp"

namespace vkf {

class PipelineManager {
public:
    inline PipelineManager(const ShaderManager& vertShaderMgr, const ShaderManager& fragShaderMgr);
};

PipelineManager::PipelineManager(const ShaderManager& vertShaderMgr, const ShaderManager& fragShaderMgr) {
    vk::GraphicsPipelineCreateInfo pipelineInfo;

    vk::PipelineInputAssemblyStateCreateInfo inputAsmStateInfo;
    inputAsmStateInfo.setTopology(vk::PrimitiveTopology::eTriangleList);
    pipelineInfo.setPInputAssemblyState(&inputAsmStateInfo);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
    vertShaderStageInfo.setStage(vk::ShaderStageFlagBits::eVertex);
    vertShaderStageInfo.setModule(vertShaderMgr.getShaderModule());
    vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
    fragShaderStageInfo.setStage(vk::ShaderStageFlagBits::eFragment);
    fragShaderStageInfo.setModule(fragShaderMgr.getShaderModule());
    const std::array shaderStageInfos{vertShaderStageInfo, fragShaderStageInfo};
    pipelineInfo.setStages(shaderStageInfos);
}

}  // namespace vkf
