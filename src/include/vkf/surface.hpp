#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "vkf/instance.hpp"
#include "vkf/window.hpp"

namespace vkf {

class SurfaceManager {
public:
    inline SurfaceManager(const InstanceManager& instanceMgr, WindowManager& windowMgr);
    inline ~SurfaceManager() noexcept;

    [[nodiscard]] inline vk::SurfaceKHR& getSurface() noexcept;
    [[nodiscard]] inline const vk::SurfaceKHR& getSurface() const noexcept;

private:
    const InstanceManager& instanceMgr_;  // FIXME: UAF
    vk::SurfaceKHR surface_;
};

SurfaceManager::SurfaceManager(const InstanceManager& instanceMgr, WindowManager& windowMgr)
    : instanceMgr_(instanceMgr) {
    glfwCreateWindowSurface(instanceMgr.getInstance(), windowMgr.getWindow(), nullptr, (VkSurfaceKHR)&surface_);
}

SurfaceManager::~SurfaceManager() noexcept { instanceMgr_.getInstance().destroy(surface_); }

vk::SurfaceKHR& SurfaceManager::getSurface() noexcept { return surface_; }

const vk::SurfaceKHR& SurfaceManager::getSurface() const noexcept { return surface_; }

}  // namespace vkf
