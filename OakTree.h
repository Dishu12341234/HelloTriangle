#ifndef OAK_TREE_H
#define OAK_TREE_H
#include "GameObjectPool.h"
#include "models/StandarBoxModel.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class OakTree
{
private:
    VulkanContext &vkContext;
    GameObjectPool &gameObjectPool;

public:
    OakTree(GameObjectPool &gameObjectPool, VulkanContext &vkContext);
    void generateTree(glm::vec3 pos);
    void generateLeafLayer(glm::vec3 pos, float height, int size);
    ~OakTree();
};

#endif