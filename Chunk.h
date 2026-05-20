#ifndef CHUNK_H
#define CHUNK_H

#include "GameObjectPool.h"
#include "models/StandarBoxModel.h"

#define LAYER_COUNT 320 // 256 + 64

<<<<<<< HEAD
struct BlockCoord
{
    int x, y, z;
    bool operator==(const BlockCoord &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
=======
#include <array>
#include <chrono>

#define TIMER_START(name)                                                      \
  auto name##_start = std::chrono::high_resolution_clock::now();

#define TIMER_END(name)                                                        \
  {                                                                            \
    auto name##_end = std::chrono::high_resolution_clock::now();               \
    auto name##_dur = std::chrono::duration_cast<std::chrono::milliseconds>(   \
                          name##_end - name##_start)                           \
                          .count();                                            \
    std::cout << #name << " took " << name##_dur << " ms\n";                   \
  }

struct Layer {
  int z;
  std::unordered_set<int> blocksTypes{0};
  Voxel voxels[16][16];
  bool hasVisible = true;
>>>>>>> 196e0be (=font rendering)
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
<<<<<<< HEAD
    GameObjectPool *gameObjectPool = nullptr;

    glm::vec2 chunkOffset{0, 0};
    std::unordered_map<BlockCoord, uint64_t, BlockCoordHash> blocks;
    GameObject *chunkMeshObject = nullptr;

    friend class Terrain;
=======
  VulkanContext &vkContext;

  std::array<Layer, 256> layers;

  int chunkOffset[2] = {0, 0}; // x,y

  Mesh<Vertex> chunkMesh;
  GameObjectPool &gop;

  friend class GameObjectPool;
  friend class Terrain;

>>>>>>> 196e0be (=font rendering)
public:
    Chunk(GameObjectPool *);
    ~Chunk();

    void setOffset(glm::vec2 chunkOffset);

    void generateChunks();
    void generateChunkMesh();
    const glm::vec2& getOffset() const { return chunkOffset; }

<<<<<<< HEAD
    std::unordered_map<BlockCoord, uint64_t, BlockCoordHash>& getBlocks()
    {
        return blocks;
    }

    const std::unordered_map<BlockCoord, uint64_t, BlockCoordHash>& getBlocks() const
    {
        return blocks;
    }
=======
  void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout,
            VkPipeline graphicsPipeline,
            std::vector<VkDescriptorSet> &descriptorSets, uint32_t currentFrame,
            VkExtent2D &swapChainExtent, PushConstantC1 &c1);

  void cleanup();
  ~Chunk() = default;
>>>>>>> 196e0be (=font rendering)
};

#endif