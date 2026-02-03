#include "HelloTriangleApplication.hpp"
#include "GameMeshObject.h"
#include "models/StandarBoxModel.h"
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
    //
    constexpr float TOP_U0 = 0.0f;
    constexpr float TOP_V0 = 0.0f;
    constexpr float TOP_U1 = U;
    constexpr float TOP_V1 = U;

    constexpr float SIDE_U0 = U;
    constexpr float SIDE_V0 = 0.0f;
    constexpr float SIDE_U1 = U * 2.0f;
    constexpr float SIDE_V1 = U;

    std::vector<Vertex> vertices = {

        // +Y (front) — top
        {{-.05, -.05, .05}, {.05, .05, .05}, {TOP_U0, TOP_V0}},
        {{.05, -.05, .05}, {.05, .05, .05}, {TOP_U1, TOP_V0}},
        {{.05, .05, .05}, {.05, .05, .05}, {TOP_U1, TOP_V1}},
        {{-.05, .05, .05}, {.05, .05, .05}, {TOP_U0, TOP_V1}},

        // -Z (back) — SIDE
        {{.05, -.05, -.05}, {.05, .05, .05}, {SIDE_U0, SIDE_V0}},
        {{-.05, -.05, -.05}, {.05, .05, .05}, {SIDE_U1, SIDE_V0}},
        {{-.05, .05, -.05}, {.05, .05, .05}, {SIDE_U1, SIDE_V1}},
        {{.05, .05, -.05}, {.05, .05, .05}, {SIDE_U0, SIDE_V1}},

        // -X (left) — SIDE
        {{-.05, -.05, -.05}, {.05, .05, .05}, {SIDE_U0, SIDE_V0}},
        {{-.05, -.05, .05}, {.05, .05, .05}, {SIDE_U1, SIDE_V0}},
        {{-.05, .05, .05}, {.05, .05, .05}, {SIDE_U1, SIDE_V1}},
        {{-.05, .05, -.05}, {.05, .05, .05}, {SIDE_U0, SIDE_V1}},

        // +X (right) — SIDE
        {{.05, -.05, .05}, {.05, .05, .05}, {SIDE_U0, SIDE_V0}},
        {{.05, -.05, -.05}, {.05, .05, .05}, {SIDE_U1, SIDE_V0}},
        {{.05, .05, -.05}, {.05, .05, .05}, {SIDE_U1, SIDE_V1}},
        {{.05, .05, .05}, {.05, .05, .05}, {SIDE_U0, SIDE_V1}},

        // +Y (top) — TOP
        {{-.05, .05, .05}, {.05, .05, .05}, {SIDE_U0, SIDE_V0}},
        {{.05, .05, .05}, {.05, .05, .05}, {SIDE_U1, SIDE_V0}},
        {{.05, .05, -.05}, {.05, .05, .05}, {SIDE_U1, SIDE_V1}},
        {{-.05, .05, -.05}, {.05, .05, .05}, {SIDE_U0, SIDE_V1}},

        // -Y (bottom) — SIDE
        {{-.05, -.05, -.05}, {.05, .05, .05}, {SIDE_U0, SIDE_V0}},
        {{.05, -.05, -.05}, {.05, .05, .05}, {SIDE_U1, SIDE_V0}},
        {{.05, -.05, .05}, {.05, .05, .05}, {SIDE_U1, SIDE_V1}},
        {{-.05, -.05, .05}, {.05, .05, .05}, {SIDE_U0, SIDE_V1}},
    };

    std::vector<uint32_t> indices = {
        // +Z (front)
        0, 1, 2,
        2, 3, 0,

        // -Z (back)
        4, 5, 6,
        6, 7, 4,

        // -X (left)
        8, 9, 10,
        10, 11, 8,

        // +X (right)
        12, 13, 14,
        14, 15, 12,

        // +Y (top)
        16, 17, 18,
        18, 19, 16,

        // -Y (bottom)
        20, 21, 22,
        22, 23, 20};
    //

    gameObjectPool.init(context);

    StandardBoxModel *grass = new StandardBoxModel({0, 1, 1, 1, 1, 1}, context);
    grass->transform.position = glm::vec3(1.f,1.f,0.f);

    gameObjectPool.appendGameObject(grass);

    gameObjectPool.uploadVBOsAndIBOs();
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

    ubo.view = glm::lookAt(glm::vec3(2.f, 2.f, 1.f), glm::vec3(0.f), glm::vec3(0.f, -0.f, 1.f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

    ubo.proj[1][1] *= -1; //-1 => y -> -y
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}