#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include "GraphicsPipeline.h"

class HelloTriangleApplication
{
private:
    VkInstance instance;

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    VkSurfaceKHR surface;
    VkQueue presentQueue;

    VkPhysicalDevice physicalDevice;
    VkDevice device;       // logical device
    VkQueue graphicsQueue; // implicitly cleaned up

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;// implicitly cleaned up
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;//explicit cleanup
    VkCommandPool commandPool;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    void initWindow();

    void initVulkan();
    void createInstance();

    bool checkValidationLayerSupport();

    void createSurface();

    void pickPhysicalDevice();
    int rateDeviceSuitablity(VkPhysicalDevice device);

    void createLogicalDevice();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    // Surface format
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capablities);

    VkRenderPass renderPass;

    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandBuffer commandBuffer;

    void createSwapChain();
    void createImageViews();

    void createRenderPass();


    u_GraphicsPipeline graphicsPipeline;

    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObject();

    void mainLoop();

    void drawFrame();

    void cleanup();

    const int WIDTH = 1080;
    const int HEIGHT = 720;

    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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