#include "HelloTriangleApplication.hpp"
#include "GameMeshObject.h"
#include "models/StandarBoxModel.h"
#include <cstdlib>
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

    StandardBoxModel *testBlock = new StandardBoxModel({0, 1, 1, 1, 1, 1}, context);
    StandardBoxModel *testBlock1 = new StandardBoxModel({0, 1, 1, 1, 1, 1}, context);
    
    gameObjectPool.appendGameObject(testBlock1);
    gameObjectPool.appendGameObject(testBlock);

    std::cout << testBlock->getID() << std::endl;
    std::cout << testBlock1->getID() << std::endl;

    testBlock->transform.position.x = 0;
    testBlock->transform.position.y = 0;
    testBlock->transform.position.z = 1.6f;

    testBlock1->transform.position.x = 0.1f;
    testBlock1->transform.position.y = 0;
    testBlock1->transform.position.z = 1.6f;

    testBlock->removeFace(LEFT);
    testBlock1->removeFace(RIGHT);


    gameObjectPool.initUpload();

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

    static bool rWasPressed = false;
    bool rPressedNow = glfwGetKey(_window, GLFW_KEY_R) == GLFW_PRESS;

    if (rPressedNow && !rWasPressed)
    {
        std::cout << "Re-upload triggered\n";
        gameObjectPool.reuploadAll();
    }

    rWasPressed = rPressedNow;

    camera->updateUBO(ubo, swapChainExtent, *event);

    glm::vec3 coordinates = camera->gePositionInWorldCoords();
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