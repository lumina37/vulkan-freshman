#pragma once

#include <print>
#include <unordered_set>
#include <vulkan/vulkan.hpp>

#include "vkf/helper/defines.hpp"
#include "vkf/helper/env.hpp"

namespace vkf {

class InstanceManager {
public:
    InstanceManager();
    ~InstanceManager();

    inline vk::Instance& getInstance() noexcept { return instance_; }
    inline const vk::Instance& getInstance() const noexcept { return instance_; }

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

    instance_ = vk::createInstance(instInfo);
};

InstanceManager::~InstanceManager() { instance_.destroy(); }

}  // namespace vkf
