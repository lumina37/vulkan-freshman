#pragma once

#include <utility>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "vkg/instance.hpp"
#include "vkg/window.hpp"

namespace vkg {

class SurfaceManager {
public:
    inline SurfaceManager(InstanceManager& instanceMgr, const WindowManager& windowMgr);
    inline ~SurfaceManager() noexcept;

    template <typename Self>
    [[nodiscard]] auto&& getSurface(this Self&& self) noexcept {
        return std::forward_like<Self>(self).surface_;
    }

private:
    InstanceManager& instanceMgr_;  // FIXME: UAF
    vk::SurfaceKHR surface_;
};

SurfaceManager::SurfaceManager(InstanceManager& instanceMgr, const WindowManager& windowMgr)
    : instanceMgr_(instanceMgr) {
    glfwCreateWindowSurface(instanceMgr.getInstance(), windowMgr.getWindow(), nullptr, (VkSurfaceKHR*)&surface_);
}

SurfaceManager::~SurfaceManager() noexcept { instanceMgr_.getInstance().destroySurfaceKHR(surface_); }

}  // namespace vkg
