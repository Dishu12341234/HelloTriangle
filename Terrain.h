#ifndef TERRAIN_H
#define TERRAIN_H
#include "GameObjectPool.h"
#include "Chunk.h"

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
    ~Terrain();
};

#endif