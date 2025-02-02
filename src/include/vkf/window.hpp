#pragma once

#include <cstdint>
#include <utility>

#include <GLFW/glfw3.h>

namespace vkf {

class WindowManager {
public:
    inline WindowManager(const vk::Extent2D& extent);
    inline WindowManager();

    static inline void globalInit() { glfwInit(); }
    static inline void globalDestroy() { glfwTerminate(); }

    [[nodiscard]] static inline std::pair<uint32_t, const char**> getExtensions();

    [[nodiscard]] inline uint32_t getWidth() const noexcept { return extent_.width; };
    [[nodiscard]] inline uint32_t getHeight() const noexcept { return extent_.height; };
    [[nodiscard]] inline vk::Extent2D getExtent() const noexcept { return extent_; };
    template <class Self>
    [[nodiscard]] inline auto&& getWindow(this Self& self) noexcept {
        return std::forward_like<Self>(self).window_;
    }

    inline void loop();

private:
    vk::Extent2D extent_;
    GLFWwindow* window_;
};

WindowManager::WindowManager(const vk::Extent2D& extent) : extent_(extent) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window_ = glfwCreateWindow((int)extent.width, (int)extent.height, "Vulkan Freshman", nullptr, nullptr);
}

WindowManager::WindowManager() { glfwDestroyWindow(window_); }

void WindowManager::loop() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
    }
}

std::pair<uint32_t, const char**> WindowManager::getExtensions() {
    uint32_t count;
    const auto pExts = glfwGetRequiredInstanceExtensions(&count);
    return {count, pExts};
}

}  // namespace vkf
