#include "Camera.h"
#include "HelloTriangleApplication.hpp"
#include <iostream>
#define DEG_TO_RAD 0.017453293

Camera::Camera(VulkanContext vkContext, GameObjectPool &gop) : gameObjectPool{gop}
{
    this->vkContext = vkContext;

    ray = gameObjectPool.createNewGameObject();
    gameObjectPool.appendGameObject(ray);
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    ray->transform.position = glm::vec3(2.f, 2.f, 6.f);

    vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{.0f, 1.f, 0.f}, TOP, 0);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{1.f, 0.f, 0.f}, TOP, 1);
    // vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, TOP, 2);
    // vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, TOP, 3);

    indices.push_back(0);
    indices.push_back(1);
    // indices.push_back(2);
    // indices.push_back(2);
    // indices.push_back(3);
    // indices.push_back(0);

    ray->loadGeometry(vertices, indices);
}

void Camera::updateUBO(UniformBufferObject &UBO,
                       VkExtent2D &swapChainExtent,
                       Event &event)
{
    // ======== Persistent State ========
    static float yaw = -90.0f;
    static float pitch = 0.0f;

    static double lastX = event.mouseX;
    static double lastY = event.mouseY;

    static float smoothDX = 0.0f;
    static float smoothDY = 0.0f;

    float dt = event.dt * 0.001f; // ms -> seconds

    // ======== Mouse Delta ========
    double dx = lastX - event.mouseX;
    double dy = lastY - event.mouseY;

    lastX = event.mouseX;
    lastY = event.mouseY;

    // ======== Mouse Smoothing ========
    float smoothing = 12.0f;
    float alpha = 1.0f - exp(-smoothing * dt);

    smoothDX += (dx - smoothDX) * alpha;
    smoothDY += (dy - smoothDY) * alpha;

    float sensitivity = 0.1f;

    yaw += smoothDX * sensitivity;
    pitch += smoothDY * sensitivity;

    pitch = glm::clamp(pitch, -89.9f, 89.9f);

    // ======== Direction Vectors ========
    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.z = sin(glm::radians(pitch));
    forward = glm::normalize(forward);

    glm::vec3 worldUp(0.0f, 0.0f, 1.0f);

    // --- Flatten forward for ground movement ---
    glm::vec3 forwardFlat = glm::normalize(
        glm::vec3(forward.x, forward.y, 0.0f));

    glm::vec3 right = glm::normalize(glm::cross(forwardFlat, worldUp));

    // ======== Movement ========
    float speed = 1.5f;
    float velocity = speed * dt;

    if (event.getKeyPressed(GLFW_KEY_W))
        cameraPos += forwardFlat * velocity;

    if (event.getKeyPressed(GLFW_KEY_S))
        cameraPos -= forwardFlat * velocity;

    if (event.getKeyPressed(GLFW_KEY_A))
        cameraPos -= right * velocity;

    if (event.getKeyPressed(GLFW_KEY_D))
        cameraPos += right * velocity;

    if (event.getKeyPressed(GLFW_KEY_SPACE))
        cameraPos.z += velocity;

    if (event.getKeyPressed(GLFW_KEY_LEFT_SHIFT))
        cameraPos.z -= velocity;

    // ======== View Matrix ========
    UBO.view = glm::lookAt(
        cameraPos,
        cameraPos + forward,
        worldUp);

    // ======== Projection Matrix ========
    UBO.proj = glm::perspective(
        glm::radians(45.0f),
        swapChainExtent.width / (float)swapChainExtent.height,
        0.1f,
        100.0f);

    UBO.proj[1][1] *= -1; // Vulkan clip correction
}

glm::vec3 Camera::gePositionInWorldCoords()
{
    return cameraPos * 10.f;
}

void Camera::drawRays(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame)
{
    auto mesh = ray->mesh;
    if (!mesh)
        return;

    if (mesh->vertexBuffer == VK_NULL_HANDLE ||
        mesh->indexBuffer == VK_NULL_HANDLE)
    {
        return; // skip this object
    }
    if (mesh->indices.size() == 0)
        return;
    // std::cout << GOID << std::endl;
    VkBuffer vertexBuffers[] = {mesh->vertexBuffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &(descriptorSets[currentFrame]), 0, nullptr);

    PushConstantC1 c1;
    c1.model = glm::translate(glm::mat4(1.f), glm::vec3(ray->transform.position));

    vkCmdPushConstants(commandBuffer, graphicsPipeline.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantC1), &c1);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh->indices.size()), 1, 0, 0, 0);
}

Camera::~Camera()
{
}
