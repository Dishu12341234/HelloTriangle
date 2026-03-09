#ifndef CAMERA_H
#define CAMERA_H

#include "GraphicsPipeline.h"
#include "Event.h"
#include "GameObjectPool.h"

struct Crosshair
{
    
};

class Camera
{
private:
    VulkanContext vkContext;
    glm::vec3 cameraPos{2.0f, 2.0f, 6.5f};
    GameObjectPool &gameObjectPool;
    GameObject *ray = nullptr;

public:
    Camera(VulkanContext vkContext, GameObjectPool &gop);
    void updateUBO(UniformBufferObject &UBO, VkExtent2D &swapChainExtent, Event &event);
    glm::vec3 gePositionInWorldCoords();

    void drawRays(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame);

    ~Camera();
};

#endif