#ifndef GAME_OBJECT_POOL
#define GAME_OBJECT_POOL

#define MAX_OBJS 65536 * 5

#include <vector>
#include <string.h>
#include <atomic>

#include <thread>

#include "GameObject.h"

struct alignas(16) ObjectData
{
    glm::mat4 model;
    uint32_t tileIndex[6];
};

class GameObjectPool
{
private:
    size_t uploadIndex = 0;
    bool uploadInProgress = false;
    VkCommandBuffer uploadCmd = VK_NULL_HANDLE;
    MeshUploader meshUploader;
    friend class Terrain;
    class Terrain *terrain = nullptr;

    friend class HelloTriangleApplication;

public:
    VulkanContext vkContext;
    std::vector<GameObject *> gameObjects;
    std::vector<PendingUpload> uploadGarbage;
    std::vector<glm::mat4> modelMatrices;

    bool ready = false;

    GameObjectPool();
    void init(VulkanContext context);
    GameObject *createNewGameObject(std::string modelPath);
    GameObject *createNewGameObject();
    void appendGameObject(GameObject *);
    void initUpload();
    void uploadChunk();
    void drawIndexed(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame);
    void reuploadAll();
    void cleanUpResources();
    ~GameObjectPool();
};

#endif