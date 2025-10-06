#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class HelloTriangleApplication
{
private:
    VkInstance instance;

    void initWindow();
    void initVulkan();
    void createInstance();
    bool checkValidationLayerSupport();
    void mainLoop();
    void cleanup();

    const int HEIGHT = 1080;
    const int WIDTH = 1920;

    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    #ifndef NDEBUG
        const bool enableValidationLayers = true;
    #else
        const bool enableValidationLayers = false;
    #endif

    GLFWwindow *_window;

public:
    HelloTriangleApplication();
    void run();
    ~HelloTriangleApplication();
};