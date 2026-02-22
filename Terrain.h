#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "GameObjectPool.h"
#include "Chunk.h"

class Terrain
{
private:
    VulkanContext vkContext;
    GameObjectPool &gameObjectPool;
    std::vector<Chunk> chunks;
    uint16_t chunkid{0};
public:
    Terrain(VulkanContext vkContext, GameObjectPool &gop);
    void generateChunks();
    StandardBoxModel* getBlock(int xBlock, int yBlock, int zBlock);
    ~Terrain();
};



#endif