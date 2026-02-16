#include "Terrain.h"
#include "models/StandarBoxModel.h"

Terrain::Terrain(VulkanContext vkContext, GameObjectPool &gop) : gameObjectPool{gop}
{
    this->vkContext = vkContext;
}

void Terrain::generateChunks()
{
    // int xn = 20;
    // int yn = 20;
    // for (int x = 0; x < xn; x++)
    // {
    //     for (int y = 0; y < yn; y++)
    //     {
    //         StandardBoxModel *sbm = new StandardBoxModel({0, 1, 1, 1, 1, 1}, vkContext);
    //         sbm->transform.position = glm::vec3(float(x) / 10, float(y) / 10, 0);
    //         sbm->tileIndex = 0;
    //         gameObjectPool.appendGameObject(sbm);
    //     }
    // }

    chunks.emplace_back(gameObjectPool, vkContext);
    for (auto &&chunk : chunks)
    {
        chunk.generateChunk();
    }
    
}

Terrain::~Terrain()
{
}
