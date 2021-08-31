#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace lve {
    class LveWindow {
        public:
            LveWindow(int width, int height, std::string windowName);
            ~LveWindow();

            LveWindow(const LveWindow &) = delete;
            LveWindow &operator =(const LveWindow &);

            bool shouldClose() { return glfwWindowShouldClose(window); }
            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        private:
            void initWindow();

            const int height, width;

            std::string windowName;
            GLFWwindow *window;
    };
} // namespace lve