#include "Chunk.h"

Chunk::Chunk(GameObjectPool &gameObjectPool, VulkanContext &vkContext) : gameObjectPool{gameObjectPool}, vkContext{vkContext}
{
}

void Chunk::generateChunk()
{
    for (size_t z = 0; z < 64; z++)
    {
        for (size_t x = 0; x < 16; x++)
        {
            for (size_t y = 0; y < 16; y++)
            {
                StandardBoxModel *sbm = new StandardBoxModel({0, 1, 1, 1, 1, 1}, vkContext);
                sbm->transform.position.x = x / 10.f;
                sbm->transform.position.y = y / 10.f;
                sbm->transform.position.z = z / 10.f - 64/10.f;
                blocks[z][x][y] = x * y * z;
                gameObjectPool.appendGameObject(sbm);
            }
        }
    }
}
