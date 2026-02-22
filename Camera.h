#ifndef CAMERA_H
#define CAMERA_H

#include "GraphicsPipeline.h"
#include "Event.h"

class Camera
{
private:
    VulkanContext vkContext;
    glm::vec3 cameraPos{2.0f, 2.0f, 0.0f};

public:
    Camera(VulkanContext vkContext);
    void updateUBO(UniformBufferObject &UBO, VkExtent2D &swapChainExtent, Event &event);
    glm::vec3 gePositionInWorldCoords();
    ~Camera();
};

#endif