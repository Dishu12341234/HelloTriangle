#ifndef PLAYER_H
#define PLAYER_H
#include "Camera.h"
#include "Event.h"
#include "Physics.h"
#include "GameObjectPool.h"

class Player
{
private:

    std::unique_ptr<Camera> camera;
    friend class HelloTriangleApplication;

public:
    Player(VulkanContext vkContext, GameObjectPool *gop);
    void updateUBO(UniformBufferObject &UBO, VkExtent2D &swapChainExtent, Event &event);
    ~Player();
};

#endif