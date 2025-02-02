#pragma once

#include <cstring>
#include <ranges>

#include <vulkan/vulkan.hpp>

namespace vkf {

namespace rgs = std::ranges;

static const char* VALIDATION_LAYER_NAME = "VK_LAYER_KHRONOS_validation";

static inline bool hasValidationLayer() noexcept {
    const auto hasValidationLayer = [](const auto& layerProp) {
        return std::strcmp(VALIDATION_LAYER_NAME, layerProp.layerName) != 0;
    };

    return rgs::any_of(vk::enumerateInstanceLayerProperties(), hasValidationLayer);
}

}  // namespace vkf