#include "Terrain.h"

Terrain::Terrain(VulkanContext &context, GameObjectPool &gop) : vkContext{context}, gameObjectPool{gop}
{
}

void Terrain::loadChunks()
{
    for (int y = -3; y <= 3; y++)
    {
        for (int x = -3; x <= 3; x++)
        {
            loadedChunks.emplace_back(&gameObjectPool);
            loadedChunks.back().setOffset({x, y});
            loadedChunks.back().generateChunks();
        }
    }
    cullAllBlocks();
}
Chunk *Terrain::getChunk(int chunkX, int chunkY)
{
    for (auto &chunk : loadedChunks)
    {
        glm::vec2 off = chunk.getOffset();
        if ((int)off.x == chunkX && (int)off.y == chunkY)
            return &chunk;
    }
    return nullptr;
}

StandardBoxModel *Terrain::getBlockFromWorld(BlockCoord world)
{
    int chunkX = world.x >> 4;
    int chunkY = world.y >> 4;

    Chunk *chunk = getChunk(chunkX, chunkY);
    if (!chunk)
        return nullptr;

    auto &blocks = chunk->getBlocks();
    auto it = blocks.find(world);

    if (it == blocks.end())
        return nullptr;

    return gameObjectPool.getBlock(it->second);
}

void Terrain::cullAllBlocks()
{
    for (auto &chunk : loadedChunks)
    {
        for (auto &[coord, id] : chunk.getBlocks())
        {
            StandardBoxModel *model = gameObjectPool.getBlock(id);
            if (model)
                cullBlock(model, coord);
        }
    }
}

void Terrain::cullBlock(StandardBoxModel *model, BlockCoord world)
{
    auto top = getBlockFromWorld({world.x, world.y, world.z + 1});
    auto bottom = getBlockFromWorld({world.x, world.y, world.z - 1});
    auto left = getBlockFromWorld({world.x + 1, world.y, world.z});
    auto right = getBlockFromWorld({world.x - 1, world.y, world.z});
    auto front = getBlockFromWorld({world.x, world.y + 1, world.z});
    auto back = getBlockFromWorld({world.x, world.y - 1, world.z});

    if (top)
    {
        top->removeFace(BOTTOM);
        model->removeFace(TOP);
    }
    if (bottom)
    {
        bottom->removeFace(TOP);
        model->removeFace(BOTTOM);
    }
    if (left)
    {
        left->removeFace(RIGHT);
        model->removeFace(LEFT);
    }
    if (right)
    {
        right->removeFace(LEFT);
        model->removeFace(RIGHT);
    }
    if (front)
    {
        front->removeFace(BACK);
        model->removeFace(FRONT);
    }
    if (back)
    {
        back->removeFace(FRONT);
        model->removeFace(BACK);
    }
}

void Terrain::drawChunks(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame)
{
    for (auto &&c : loadedChunks)
    {

        if (c.chunkMeshObject)
            c.chunkMeshObject->drawIndexed(commandBuffer, descriptorSets, graphicsPipeline, swapChainExtent, instanceCount, currentFrame);
    }
}

Terrain::~Terrain()
{
    for (auto &&chunk : loadedChunks)
    {
        // delete chunk;
        // chunk = nullptr;
    }
}
