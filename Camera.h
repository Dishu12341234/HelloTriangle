#ifndef CAMERA_H
#define CAMERA_H

#include "GraphicsPipeline.h"
#include "Event.h"

class Camera
{
private:
    VulkanContext vkContext;

public:
    Camera(VulkanContext vkContext);
    void updateUBO(UniformBufferObject &UBO, VkExtent2D &swapChainExtent, Event &event);
    ~Camera();
};

#endif