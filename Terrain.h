#ifndef TERRAIN_H
#define TERRAIN_H
#include "GameObjectPool.h"

class Terrain
{
private:
    VulkanContext vkContext;
    GameObjectPool &gameObjectPool;
public:
    Terrain(VulkanContext vkContext, GameObjectPool &gop);
    void generateGrassLayer();
    ~Terrain();
};



#endif