#ifndef CHUNK_H
#define CHUNK_H

#define ZD 4

#include <optional>

#include "models/StandarBoxModel.h"
#include "GameObjectPool.h"

struct Layer
{
    StandardBoxModel *blocks[16][16]{nullptr};
    int z;
};

class Chunk
{
private:
    //
    Layer layers[256];
    uint16_t chunkId{UINT16_MAX};
    GameObjectPool &gameObjectPool;
    VulkanContext &vkContext;
    glm::vec2 offset{0, 0}; // offset in chunk index not it position

    friend class Terrain;

public:
    Chunk(GameObjectPool &gameObjectPool, VulkanContext &vkContext);
    void generateChunk();
    ~Chunk() = default;
};

#endif