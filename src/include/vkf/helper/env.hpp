#pragma once

#include <cstring>

#include <vulkan/vulkan.hpp>

namespace vkf {

static const char* VALIDATION_LAYER_NAME = "VK_LAYER_KHRONOS_validation";

static inline bool hasValidationLayer() noexcept {
    for (const auto& layerProp : vk::enumerateInstanceLayerProperties()) {
        if (std::strcmp(VALIDATION_LAYER_NAME, layerProp.layerName)) {
            return true;
        }
    }
    return false;
}

}  // namespace vkf