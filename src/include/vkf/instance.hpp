#pragma once

#include <print>

#include <vulkan/vulkan.hpp>

#include "vkf/helper/defines.hpp"
#include "vkf/helper/env.hpp"
#include "vkf/window.hpp"

namespace vkf {

class InstanceManager {
public:
    inline InstanceManager();
    inline ~InstanceManager() noexcept;

    template <class Self>
    [[nodiscard]] inline auto&& getInstance(this Self& self) noexcept {
        return std::forward_like<Self>(self).instance_;
    }

private:
    vk::Instance instance_;
};

InstanceManager::InstanceManager() {
    constexpr bool ENABLE_VALIDATION_LAYER = ENABLE_DEBUG;

    vk::ApplicationInfo appInfo;
    appInfo.setPApplicationName("vk-freshman");
    appInfo.setApiVersion(VK_API_VERSION_1_3);

    vk::InstanceCreateInfo instInfo;
    instInfo.setPApplicationInfo(&appInfo);

    if constexpr (ENABLE_VALIDATION_LAYER) {
        if (hasValidationLayer()) {
            instInfo.setPEnabledLayerNames({VALIDATION_LAYER_NAME});
        }
    }

    const auto [count, pExts] = WindowManager::getExtensions();
    instInfo.setEnabledExtensionCount(count);
    instInfo.setPpEnabledExtensionNames(pExts);

    instance_ = vk::createInstance(instInfo);
};

InstanceManager::~InstanceManager() noexcept { instance_.destroy(); }

}  // namespace vkf
