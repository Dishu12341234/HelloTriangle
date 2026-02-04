#include "Terrain.h"
#include "models/StandarBoxModel.h"

Terrain::Terrain(VulkanContext vkContext, GameObjectPool &gop) : gameObjectPool{gop}
{
    this->vkContext = vkContext;
}

void Terrain::generateGrassLayer()
{
    int xn = 60;
    int yn = 60;
    for (int x = 0; x < xn; x++)
    {
        for (int y = 0; y < yn; y++)
        {
            StandardBoxModel *sbm = new StandardBoxModel(GRASS_UVS, vkContext);
            sbm->transform.position = glm::vec3(float(x) / 10, float(y) / 10, 0);
            gameObjectPool.appendGameObject(sbm);
        }
    }
}

Terrain::~Terrain()
{
}
