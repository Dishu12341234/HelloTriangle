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
public:
    Terrain(VulkanContext vkContext, GameObjectPool &gop);
    void generateChunks();
    ~Terrain();
};



#endif