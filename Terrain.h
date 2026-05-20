#ifndef TERRAIN_H
#define TERRAIN_H
#include "GameObjectPool.h"
<<<<<<< HEAD
#include "Chunk.h"
=======
#include <atomic>
#include <cstdint>
#include <mutex>
#include <set>
#include <thread>
#include <vector>
#define RENDER_DISTANCE 3
>>>>>>> 196e0be (=font rendering)

#define RENDER_DISTANCE 1

class Terrain
{
private:
    VulkanContext &vkContext;
    GameObjectPool &gameObjectPool;
    std::vector<Chunk> loadedChunks;

    
public:
    Terrain(VulkanContext &context, GameObjectPool &gop);
    void loadChunks();
    Chunk *getChunk(int chunkX, int chunkY);

    StandardBoxModel *getBlockFromWorld(BlockCoord world);

    void cullAllBlocks();
    void cullBlock(StandardBoxModel *model, BlockCoord world);
    void drawChunks(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame);
    ~Terrain();
};

#endif