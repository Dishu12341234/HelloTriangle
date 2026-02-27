#ifndef CHUNK_H
#define CHUNK_H

#include "GameObjectPool.h"
#include "models/StandarBoxModel.h"

#define LAYER_COUNT 320 // 256 + 64

struct BlockCoord
{
    int x, y, z;
    bool operator==(const BlockCoord &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct BlockCoordHash
{
    size_t operator()(const BlockCoord &c) const noexcept
    {
        size_t h1 = std::hash<int>{}(c.x);
        size_t h2 = std::hash<int>{}(c.y);
        size_t h3 = std::hash<int>{}(c.z);
        return h1 ^ (h2 << 4) ^ (h3 << 8); // simple but effective
    }
};

class Chunk
{
private:
    GameObjectPool *gameObjectPool = nullptr;

    glm::vec2 chunkOffset{0, 0};
    std::unordered_map<BlockCoord, uint64_t, BlockCoordHash> blocks;
    GameObject *chunkMeshObject = nullptr;

    friend class Terrain;
public:
    Chunk(GameObjectPool *);
    ~Chunk();

    void setOffset(glm::vec2 chunkOffset);

    void generateChunks();
    void generateChunkMesh();
    const glm::vec2& getOffset() const { return chunkOffset; }

    std::unordered_map<BlockCoord, uint64_t, BlockCoordHash>& getBlocks()
    {
        return blocks;
    }

    const std::unordered_map<BlockCoord, uint64_t, BlockCoordHash>& getBlocks() const
    {
        return blocks;
    }
};

#endif