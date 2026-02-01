#include "HelloTriangleApplication.hpp"
#include "GameMeshObject.h"
#include <cstdlib>
#include <cstring>
#include <memory>


void HelloTriangleApplication::initGameObjects()
{
    VulkanContext context{};
    context.device = device;
    context.physicalDevice = physicalDevice;
    context.graphicsQueue = graphicsQueue;
    context.instance = instance;
    context.presentQueue = presentQueue;
    context.commandPool = commandPool;

    gameObjectPool.init(context);

    GameObject* box = gameObjectPool.createNewGameObject(MODEL_PATH);
    gameObjectPool.appendGameObject(box);

    gameObjectPool.uploadVBOsAndIBOs();
}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};

    ubo.model[0] = glm::translate(glm::mat4(1.f), glm::vec3(-2.f, .2f, -.1f));
    ubo.model[0] = glm::rotate(
        ubo.model[0],
        time * glm::radians(90.f),
        glm::vec3(0.f, 0.f, 1.f));

    // Model 1
    ubo.model[1] = glm::translate(glm::mat4(1.f), glm::vec3(.5f, -.5f, 0.f));
    ubo.model[1] = glm::rotate(
        ubo.model[1],
        time * glm::radians(90.f),
        glm::vec3(0.f, 0.f, 1.f));

    // Model 2
    ubo.model[2] = glm::translate(glm::mat4(1.f), glm::vec3(-1.f, 0.f, -1.2f));
    ubo.model[2] = glm::rotate(
        ubo.model[2],
        time * glm::radians(90.f),
        glm::vec3(0.f, 0.f, 1.f));

    ubo.view = glm::lookAt(glm::vec3(2.f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

    ubo.proj[1][1] *= -1; //-1 => y -> -y
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}