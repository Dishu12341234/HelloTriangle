#include "HelloTriangleApplication.hpp"
#include "GameMeshObject.h"
#include "models/StandarBoxModel.h"
#include <cstdlib>
#include "Terrain.h"
#include "OakTree.h"
#include <cstring>
#include <memory>

#define DEG_TO_RAD 0.017453293

void HelloTriangleApplication::initGameObjects()
{
    // ===== INIT =====
    VulkanContext context{};
    context.device = device;
    context.physicalDevice = physicalDevice;
    context.graphicsQueue = graphicsQueue;
    context.instance = instance;
    context.presentQueue = presentQueue;
    context.commandPool = commandPool;

    camera = new Camera(context);
    gameObjectPool.init(context);

    // Generation

    OakTree tree(gameObjectPool, context);
    tree.generateTree({0, 0, 0});

    terrain = new Terrain(context, gameObjectPool);
    terrain->generateChunks();

    // upload
    // std::cout << "Before" << std::endl;
    // std::cout << "After" << std::endl;

    StandardBoxModel *testBlock = new StandardBoxModel({0, 1, 1, 1, 1, 1}, context);
    testBlock->removeFace(TOP);
    std::cout << testBlock->getID() << std::endl;

    testBlock->transform.position.x = 0;
    testBlock->transform.position.y = 0;
    testBlock->transform.position.z = 1.6f;

    gameObjectPool.appendGameObject(testBlock);

    gameObjectPool.initUpload();
    // std::cout << "Before: " << testBlock->mesh->vertices.size() << std::endl;
    // testBlock->removeFace(Face::TOP);
    // std::cout << "After: " << testBlock->mesh->vertices.size() << std::endl;
    // TODO add faces on demand not as default
}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::translate(glm::mat4(1.f), glm::vec3(1, 1, 1));
    int key = glfwGetKey(_window, GLFW_KEY_ESCAPE);
    if (key == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_window, true);
        menu = !menu;
        if (menu)
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    camera->updateUBO(ubo, swapChainExtent, *event);
    
    glm::vec3 coordinates = camera->gePositionInWorldCoords();
    auto block = terrain->getBlock(coordinates.x, coordinates.y, coordinates.z);
    if (block)
    {
        std::cout << block->getID() << std::endl;
    }
    std::cout << "(x,y,z):(" << coordinates.x << "," << coordinates.y << "," << coordinates.z << ")" << std::endl;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void HelloTriangleApplication::updateShaderStorageBuffer(uint32_t currentFrame)
{
    VkDeviceSize bufferSize = sizeof(ObjectData) * gameObjectPool.gameObjects.size();

    ObjectData *data;
    vkMapMemory(device,
                ssboBuffersMemory[currentFrame],
                0,
                bufferSize,
                0,
                (void **)&data);

    for (size_t i = 0; i < gameObjectPool.gameObjects.size(); i++)
    {
        data[i].model = glm::translate(glm::mat4(1.f), gameObjectPool.gameObjects.at(i)->transform.position);
        data[i].tileIndex[0] = (gameObjectPool.gameObjects.at(i)->faceUVTextureOffsets[0]);
        data[i].tileIndex[1] = (gameObjectPool.gameObjects.at(i)->faceUVTextureOffsets[1]);
        data[i].tileIndex[2] = (gameObjectPool.gameObjects.at(i)->faceUVTextureOffsets[2]);
        data[i].tileIndex[3] = (gameObjectPool.gameObjects.at(i)->faceUVTextureOffsets[3]);
        data[i].tileIndex[4] = (gameObjectPool.gameObjects.at(i)->faceUVTextureOffsets[4]);
        data[i].tileIndex[5] = (gameObjectPool.gameObjects.at(i)->faceUVTextureOffsets[5]);
    }

    vkUnmapMemory(device, ssboBuffersMemory[currentFrame]);
}