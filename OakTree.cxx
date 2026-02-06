#include "OakTree.h"

OakTree::OakTree(GameObjectPool &gameObjectPool, VulkanContext &vkContext) : vkContext{vkContext}, gameObjectPool{gameObjectPool}
{
}

void OakTree::generateTree(glm::vec3 pos)
{
    // Generate Trunk
    int nWood = rand() % 3 + 5; // 5 min and 7(5 + 3 - 1) max
    for (float z = 0; z < nWood; z++)
    {
        StandardBoxModel *sbm = new StandardBoxModel(WOOD_OAK_UVS, vkContext);
        sbm->transform.position = pos + glm::vec3(0, 0, z / 10);
        gameObjectPool.appendGameObject(sbm);
    }

    // Generate Leaves
    int nL1Leaves = rand() % 5 + 25;
    int rL1 = 0;
    for (int i = 0; i < nL1Leaves; ++i)
    {
        std::cout << i << std::endl;

        auto *sbm = new StandardBoxModel(LEAF_OAK_UVS, vkContext);
        sbm->transform.position =
            pos + glm::vec3((int(i) % 5) / 10.f, rL1 / 10.f, nWood * 0.1f) - glm::vec3(.2f, .3f, 0);
        if (i % 5 == 0)
            rL1++;

        gameObjectPool.appendGameObject(sbm);
    }
    int nL2Leaves = rand() % 5 + 16;
    int rL2 = 0;
    for (int i = 0; i < nL2Leaves; ++i)
    {
        std::cout << i << std::endl;

        auto *sbm = new StandardBoxModel(LEAF_OAK_UVS, vkContext);
        sbm->transform.position =
            pos + glm::vec3(rL2 / 10.f, (int(i) % 3) / 10.f, (nWood + 1) * 0.1f) - glm::vec3(.2f, .1f, 0);
        if (i % 5 == 0)
            rL2++;

        gameObjectPool.appendGameObject(sbm);
    }
    int nL3Leaves = rand() % 5 + 4;
    int rL3 = 0;
    for (int i = 0; i < nL3Leaves; ++i)
    {
        std::cout << i << std::endl;

        auto *sbm = new StandardBoxModel(LEAF_OAK_UVS, vkContext);
        sbm->transform.position =
            pos + glm::vec3(rL3 / 10.f, (int(i) % 2) / 10.f, (nWood + 2) * 0.1f) - glm::vec3(.1f, .1f, 0);
        if (i % 5 == 0)
            rL3++;

        gameObjectPool.appendGameObject(sbm);
    }
}

OakTree::~OakTree()
{
}
