#include "Chunk.h"

Chunk::Chunk(GameObjectPool *GOP) : gameObjectPool{GOP}
{
    if (!GOP)
        throw std::runtime_error("Invalid GameObjectPool for Chunk Creation");
}

Chunk::~Chunk()
{
}

void Chunk::setOffset(glm::vec2 chunkOffset)
{
    this->chunkOffset = chunkOffset;
}

void Chunk::generateChunks()
{
    for (size_t k = 0; k < LAYER_COUNT; k++)
    {
        if (k < 3)
            for (size_t i = 0; i < 16; i++)
            {
                for (size_t j = 0; j < 16; j++)
                {
                    BlockCoord coord{i + 16 * chunkOffset.x, j + 16 * chunkOffset.y, k};
                    uint64_t blockID = gameObjectPool->createNewBoxModelAndAppend({coord.x, coord.y, coord.z}, {0, 1, 1, 1, 1, 1})->getID();

                    // Emplace (won’t overwrite existing)
                    auto [it, inserted] = blocks.emplace(coord, blockID);
                }
            }
    }
    for (int k = 0; k < LAYER_COUNT; k++)
    {
        if (k < 3)
            for (int i = 0; i < 16; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    cullHiddenFaces(getBlockFromCoords(BlockCoord{i + int(16 * chunkOffset.x), j + int(16 * chunkOffset.y), k}, *this, *gameObjectPool));
                }
            }
    }
}
void Chunk::cullHiddenFaces(StandardBoxModel *model)
{
    int blockX = model->transform.position.x * 10.f - 16 * chunkOffset.x;
    int blockY = model->transform.position.y * 10.f - 16 * chunkOffset.y;
    int blockZ = model->transform.position.z * 10.f;

    auto top = getBlockFromCoords({blockX, blockY, blockZ + 1}, (*this), *gameObjectPool);
    auto bottom = getBlockFromCoords({blockX, blockY, blockZ - 1}, (*this), *gameObjectPool);
    auto left = getBlockFromCoords({blockX + 1, blockY, blockZ}, (*this), *gameObjectPool);
    auto right = getBlockFromCoords({blockX - 1, blockY, blockZ}, (*this), *gameObjectPool);
    auto front = getBlockFromCoords({blockX, blockY + 1, blockZ}, (*this), *gameObjectPool);
    auto back = getBlockFromCoords({blockX, blockY - 1, blockZ}, (*this), *gameObjectPool);

    if (top)
    {
        top->removeFace(BOTTOM); // remove bottom face of top neighbor
        model->removeFace(TOP);  // remove top face of current model
    }

    if (bottom)
    {
        bottom->removeFace(TOP);   // remove top face of bottom neighbor
        model->removeFace(BOTTOM); // remove bottom face of current model
    }

    if (left)
    {
        left->removeFace(RIGHT); // remove right face of left neighbor
        model->removeFace(LEFT); // remove left face of current model
    }

    if (right)
    {
        right->removeFace(LEFT);  // remove left face of right neighbor
        model->removeFace(RIGHT); // remove right face of current model
    }

    if (front)
    {
        front->removeFace(BACK);  // remove back face of front neighbor
        model->removeFace(FRONT); // remove front face of current model
    }

    if (back)
    {
        back->removeFace(FRONT); // remove front face of back neighbor
        model->removeFace(BACK); // remove back face of current model
    }
}
StandardBoxModel *Chunk::getBlockFromCoords(BlockCoord coords, Chunk &chunk, GameObjectPool &gop)
{
    auto it = chunk.blocks.find(coords);
    if (it != chunk.blocks.end())
        return gop.getBlock(it->second);
    return nullptr;
}