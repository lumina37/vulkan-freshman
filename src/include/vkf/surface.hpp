#pragma once

#include <utility>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "vkf/instance.hpp"
#include "vkf/window.hpp"

namespace vkf {

class SurfaceManager {
public:
    inline SurfaceManager(const InstanceManager& instanceMgr, WindowManager& windowMgr);
    inline ~SurfaceManager() noexcept;

    template <class Self>
    [[nodiscard]] inline auto&& getSurface(this Self& self) noexcept {
        return std::forward_like<Self>(self).surface_;
    }

private:
    const InstanceManager& instanceMgr_;  // FIXME: UAF
    vk::SurfaceKHR surface_;
};

SurfaceManager::SurfaceManager(const InstanceManager& instanceMgr, WindowManager& windowMgr)
    : instanceMgr_(instanceMgr) {
    glfwCreateWindowSurface(instanceMgr.getInstance(), windowMgr.getWindow(), nullptr, (VkSurfaceKHR)&surface_);
}

SurfaceManager::~SurfaceManager() noexcept { instanceMgr_.getInstance().destroySurfaceKHR(surface_); }

}  // namespace vkf
