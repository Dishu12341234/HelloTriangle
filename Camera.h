#ifndef CAMERA_H
#define CAMERA_H

#include "GraphicsPipeline.h"
#include "Event.h"
#include "GameObjectPool.h"

struct Crosshair
{
};

class Ray
{
public:
    Ray(VulkanContext vkContext, GameObjectPool &gop);

    GameObjectPool &gameObjectPool;
    GameObject *ray;
    glm::vec3 worldUp{0, 0, 1};
    glm::vec3 rayOriginCam;
    glm::vec3 rayDir;
    glm::vec3 rayOriginVox;
    float maxDistVox;

    glm::ivec3 voxel;
    glm::vec3 deltaDist;
    glm::ivec3 step;
    glm::vec3 sideDist;

    bool hit = false;
    float distanceVox = 0.0f;

    float rayLengthCam;

    glm::mat4 model;
    glm::mat4 rotation{1.f};

    void drawRay(glm::vec3 rayOrigin, int rayLength, glm::vec3 forward, bool followCamera, VkCommandBuffer &commandBuffer,
                 std::vector<VkDescriptorSet> &descriptorSets,
                 u_GraphicsPipeline &graphicsPipeline,
                 VkExtent2D &swapChainExtent,
                 uint64_t instanceCount,
                 uint32_t &currentFrame);
    bool isVoxelSolid(int x, int y, int z);
};

class Camera
{
private:
    VulkanContext vkContext;
    glm::vec3 cameraPos{2.0f, 2.0f, 6.5f};
    float pitch;
    float yaw;
    GameObjectPool &gameObjectPool;
    GameObject *ray = nullptr;
    glm::vec3 forward;
    glm::vec3 worldUp{0, 0, 1};

    Ray rayCamera;

public:
    Camera(VulkanContext vkContext, GameObjectPool &gop);
    void updateUBO(UniformBufferObject &UBO, VkExtent2D &swapChainExtent, Event &event);
    glm::vec3 gePositionInWorldCoords();

    void drawRays(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame);
    bool isVoxelSolid(int x, int y, int z);
    ~Camera();
};

#endif