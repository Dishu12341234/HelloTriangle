#include "Chunk.h"

Chunk::Chunk(GameObjectPool &gameObjectPool, VulkanContext &vkContext) : gameObjectPool{gameObjectPool}, vkContext{vkContext}
{
}

void Chunk::generateChunk()
{

    for (uint32_t z = 0; z < 64; z++)
    {
        for (size_t x = 0; x < 16; x++)
        {
            for (size_t y = 0; y < 16; y++)
            {
                StandardBoxModel *sbm = new StandardBoxModel({0, 1, 1, 1, 1, 1}, vkContext);
                sbm->transform.position.x = x / 10.f + offset.x * 1.6f;
                sbm->transform.position.y = y / 10.f + offset.y * 1.6f;
                sbm->transform.position.z = z / 10.f - 64 / 10.f;
                // sbm->removeFace(TOP);
                layers[63 - z].blocks[x][y] = sbm;
                layers[63 - z].z = z - 64;
                gameObjectPool.appendGameObject(sbm);
            }
        }
    }
}
