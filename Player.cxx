#include "Player.h"

Player::Player(VulkanContext vkContext, GameObjectPool *gop)
{
    camera = std::make_unique<Camera>(vkContext, *gop);
}

void Player::updateUBO(UniformBufferObject &UBO, VkExtent2D &swapChainExtent, Event &event)
{
    camera->updateUBO(UBO, swapChainExtent, event);
}

Player::~Player()
{
}
