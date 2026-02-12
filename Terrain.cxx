#include "Terrain.h"
#include "models/StandarBoxModel.h"

Terrain::Terrain(VulkanContext vkContext, GameObjectPool &gop) : gameObjectPool{gop}
{
    this->vkContext = vkContext;
}

void Terrain::generateGrassLayer()
{
    int xn = 255;
    int yn = 255;
    for (int x = 0; x < xn; x++)
    {
        for (int y = 0; y < yn; y++)
        {
            std::array<float, 6> uv = GRASS_UVS;
            if ((x + y) % 2 == 0)
                uv = WOOD_OAK_UVS;
            StandardBoxModel *sbm = new StandardBoxModel(uv, vkContext);
            sbm->transform.position = glm::vec3(float(x) / 10, float(y) / 10, 0);
            sbm->tileIndex = 0;
            gameObjectPool.appendGameObject(sbm);
        }
    }
}

Terrain::~Terrain()
{
}
