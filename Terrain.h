#ifndef TERRAIN_H
#define TERRAIN_H

#include <unordered_map>
#include "GameObjectPool.h"
#include "Chunk.h"

struct PairHash
{
    std::size_t operator()(const std::pair<int,int>& p) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(p.first);
        std::size_t h2 = std::hash<int>{}(p.second);

        return h1 ^ (h2 << 1);  // simple and good enough
    }
};

class Terrain
{
private:
    VulkanContext vkContext;
    GameObjectPool &gameObjectPool;
    std::unordered_map<std::pair<int, int>, Chunk, PairHash> chunks;
    uint16_t chunkid{0};

public:
    Terrain(VulkanContext vkContext, GameObjectPool &gop);

    void generateChunks();
    Chunk *getChunk(int x, int y);
    void appendBlockToTerrain(GameObject *gameObject);
    StandardBoxModel *getBlock(int xBlock, int yBlock, int zBlock);

    ~Terrain();
};

#endif