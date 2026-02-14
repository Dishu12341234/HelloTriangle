#include "OakTree.h"

OakTree::OakTree(GameObjectPool &gameObjectPool, VulkanContext &vkContext) : vkContext{vkContext}, gameObjectPool{gameObjectPool}
{
}
void OakTree::generateTree(glm::vec3 pos)
{
    // ---------------- TRUNK ----------------
    int nWood = rand() % 3 + 5;

    for (int z = 0; z < nWood; z++)
    {
        auto *sbm = new StandardBoxModel(WOOD_OAK_UVS, vkContext);
        sbm->transform.position = pos + glm::vec3(0, 0, z * 0.1f);
        sbm->tileIndex = 3;
        gameObjectPool.appendGameObject(sbm);
    }

    float baseHeight = nWood * 0.1f;

    // ---------------- LAYER 1 (5x5) ----------------
    generateLeafLayer(pos, baseHeight, 5);

    // ---------------- LAYER 2 (3x3) ----------------
    generateLeafLayer(pos, baseHeight + 0.2f, 3);

    // ---------------- LAYER 3 (2x2) ----------------
    generateLeafLayer(pos, baseHeight + 0.3f, 2);
}
void OakTree::generateLeafLayer(glm::vec3 pos, float height, int size)
{
    float half = (size - 1) * 0.05f;

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            auto *sbm = new StandardBoxModel(LEAF_OAK_UVS, vkContext);

            sbm->transform.position =
                pos +
                glm::vec3(x * 0.1f - half,
                          y * 0.1f - half,
                          height);

            sbm->tileIndex = 4;
            gameObjectPool.appendGameObject(sbm);
        }
    }
}


OakTree::~OakTree()
{
}
