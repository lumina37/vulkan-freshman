#pragma once

#include <GLFW/glfw3.h>

namespace vkf {

class WindowManager {
   public:
    inline WindowManager(int width, int height);
    inline WindowManager();

    static inline void globalInit() { glfwInit(); }
    static inline void globalDestroy() { glfwTerminate(); }

    inline void loop();

   private:
    GLFWwindow* window_;
};

WindowManager::WindowManager(int width, int height) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window_ = glfwCreateWindow(width, height, "Vulkan Freshman", NULL, NULL);
}

WindowManager::WindowManager() { glfwDestroyWindow(window_); }

void WindowManager::loop() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
    }
}

}  // namespace vkf
