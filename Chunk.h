#ifndef CHUNK_H
#define CHUNK_H

#include <optional>

#include "models/StandarBoxModel.h"
#include "GameObjectPool.h"

class Chunk
{
private:
    //              z   x   y
    uint16_t blocks[64][16][16];
    GameObjectPool &gameObjectPool;
    VulkanContext &vkContext;

    friend class Chunk;

public:
    Chunk(GameObjectPool &gameObjectPool, VulkanContext &vkContext);
    void generateChunk();
    ~Chunk() = default;
};

#endif