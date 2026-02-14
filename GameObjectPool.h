#ifndef GAME_OBJECT_POOL
#define GAME_OBJECT_POOL


#define MAX_OBJS 65536

#include <vector>
#include <string.h>
#include "GameObject.h"

struct alignas(16) ObjectData {
    glm::mat4 model;
    uint32_t tileIndex;
    glm::vec2 padding;
};


class GameObjectPool
{

public:
    VulkanContext vkContext;
    std::vector<GameObject *> gameObjects;
    std::vector<PendingUpload> uploadGarbage;
    std::vector<glm::mat4> modelMatrices;

    GameObjectPool();
    void init(VulkanContext context);
    GameObject *createNewGameObject(std::string modelPath);
    GameObject *createNewGameObject();
    void appendGameObject(GameObject *);
    void uploadVBOsAndIBOs();
    void drawIndexed(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame);
    void cleanUpResources();
    ~GameObjectPool();
};

#endif