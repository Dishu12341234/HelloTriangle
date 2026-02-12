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
    VulkanContext context{};
    context.device = device;
    context.physicalDevice = physicalDevice;
    context.graphicsQueue = graphicsQueue;
    context.instance = instance;
    context.presentQueue = presentQueue;
    context.commandPool = commandPool;

    gameObjectPool.init(context);

    Terrain terrain(context, gameObjectPool);
    terrain.generateGrassLayer();

    OakTree tree(gameObjectPool, context);
    tree.generateTree({0, 0, 0});

    // StandardBoxModel *grass = new StandardBoxModel({0, 1, 1, 1, 1, 1}, context);
    // grass->transform.position = glm::vec3(1.f,1.f,0.f);

    // gameObjectPool.appendGameObject(grass);

    std::cout << "Before" << std::endl;
    gameObjectPool.uploadVBOsAndIBOs();
    std::cout << "After" << std::endl;
}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};

    // Model 0
    // ubo.model = glm::rotate(glm::mat4(1.f), time * glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
    // ubo.model = glm::rotate(ubo.model, 3 * time * glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));

    static float yaw = -90.0f; // look forward
    static float pitch = 0.0f;
    static double lastX = WIDTH * 0.5;
    static double lastY = HEIGHT * 0.5;

    static float cposx = 2.f;
    static float cposy = 2.f;
    static float cposz = 0.f;

    double x, y;
    x = event->mouseX;
    y = event->mouseY;
    int key = glfwGetKey(_window, GLFW_KEY_ESCAPE);
    if (key == GLFW_PRESS)
    {
        menu = !menu;
        if (menu)
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    glm::vec3 worldUp(0.0f, 0.0f, 1.0f);
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    // std::cout << cposx << std::endl;

    double dx = lastX - x;
    double dy = lastY - y;

    lastX = x;
    lastY = y;

    float sensitivity = 0.1f;
    yaw += dx * sensitivity;
    pitch += dy * sensitivity;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.z = sin(glm::radians(pitch));

    forward = glm::normalize(forward);

    std::cout << "Yaw: " << yaw << std::endl;
    std::cout << "Pitch: " << pitch << std::endl;
    double dc = (DEG_TO_RAD * yaw);
    std::cout << "cos(2 * 3.1415 * yaw / 360): " << cos(dc) << std::endl;
    std::cout << "sin(2 * 3.1415 * yaw / 360): " << sin(dc) << std::endl;

    if (event->getKeyPressed(GLFW_KEY_A))
    {
        std::cout << "A" << std::endl;
        cposx -= .05f * sin(dc);
        cposy += .05f * cos(dc);
    }
    if (event->getKeyPressed(GLFW_KEY_D))
    {
        std::cout << "D" << std::endl;
        cposx += .05f * sin(dc);
        cposy -= .05f * cos(dc);
    }
    if (event->getKeyPressed(GLFW_KEY_W))
    {
        std::cout << "W" << std::endl;
        cposx += .05f * cos(dc);
        cposy += .05f * sin(dc);
    }
    if (event->getKeyPressed(GLFW_KEY_S))
    {
        std::cout << "S" << std::endl;
        cposx -= .05f * cos(dc);
        cposy -= .05f * sin(dc);
    }
    if (event->getKeyPressed(GLFW_KEY_SPACE))
    {
        cposz += .05f;
    }

    if (event->getKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        cposz -= .05f;
    }

    // std::cout << "(x,y)" << "(" << x << "," << y << ")" << std::endl;
    glm::vec3 cameraPos = glm::vec3(cposx, cposy, cposz);

    ubo.view = glm::lookAt(
        cameraPos,
        cameraPos + forward,
        glm::vec3(0.f, 0.f, 1.f));

    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

    ubo.proj[1][1] *= -1; //-1 => y -> -y
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
        data[i].uv =  StandardBoxModel::tileIndexToUV(gameObjectPool.gameObjects.at(i)->tileIndex);
    }

    vkUnmapMemory(device, ssboBuffersMemory[currentFrame]);
}
