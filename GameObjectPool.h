#ifndef GAME_OBJECT_POOL
#define GAME_OBJECT_POOL

#define MAX_OBJS 65536 * 5

#include <unordered_map>
#include <string.h>
#include <atomic>

#include <thread>

#include "GameObject.h"
#include "models/StandarBoxModel.h"

class Terrain;
struct BlockCoord;

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
    Terrain *terrain;

    friend class HelloTriangleApplication;

public:
    VulkanContext vkContext;
    std::unordered_map<uint64_t, GameObject *> gameObjects;
    std::vector<PendingUpload> uploadGarbage;
    std::vector<glm::mat4> modelMatrices;

    bool ready = false;

    GameObjectPool();
    void init(VulkanContext context);
    GameObject *createNewGameObject(std::string modelPath);
    GameObject *createNewGameObject();

    StandardBoxModel *createNewBoxModelAndAppend(glm::vec3 blockCoord, std::vector<uint32_t> faceUVTextureOffsets);
    StandardBoxModel *getBlock(uint64_t goid);

    void appendGameObject(GameObject *);
    void initUpload();
    void uploadChunk();
    void drawIndexed(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame);
    void reuploadAll();

    StandardBoxModel* getBlockFromWorldCoords(BlockCoord);

    void cleanUpResources();
    ~GameObjectPool();
};

#endif