#include "Camera.h"
#include "HelloTriangleApplication.hpp"
#include <iostream>
#define DEG_TO_RAD 0.017453293


Camera::Camera(VulkanContext vkContext)
{
    this->vkContext = vkContext;
}

void Camera::updateUBO(UniformBufferObject &UBO,
                       VkExtent2D &swapChainExtent,
                       Event &event)
{
    // ======== Persistent State ========
    static float yaw   = -90.0f;
    static float pitch = 0.0f;

    static glm::vec3 cameraPos(2.0f, 2.0f, 0.0f);

    static double lastX = event.mouseX;
    static double lastY = event.mouseY;

    static float smoothDX = 0.0f;
    static float smoothDY = 0.0f;

    // ======== Mouse Delta ========
    double dx = lastX - event.mouseX;
    double dy = lastY - event.mouseY; // invert Y (FPS style)

    lastX = event.mouseX;
    lastY = event.mouseY;

    float dt = event.dt * 0.001f; // ms -> seconds

    // ======== Mouse Smoothing ========
    float smoothing = 12.0f; // tune 8–20
    float alpha = 1.0f - exp(-smoothing * dt);

    smoothDX += (dx - smoothDX) * alpha;
    smoothDY += (dy - smoothDY) * alpha;

    float sensitivity = 0.1f;

    yaw   += smoothDX * sensitivity;
    pitch += smoothDY * sensitivity;

    pitch = glm::clamp(pitch, -89.9f, 89.9f);

    // ======== Direction Vectors ========
    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.z = sin(glm::radians(pitch));
    forward   = glm::normalize(forward);

    glm::vec3 worldUp(0.0f, 0.0f, 1.0f);
    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));

    // ======== Movement ========
    float speed = 5.0f; // units per second
    float velocity = speed * dt;

    if (event.getKeyPressed(GLFW_KEY_W))
        cameraPos += forward * velocity;

    if (event.getKeyPressed(GLFW_KEY_S))
        cameraPos -= forward * velocity;

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
    

Camera::~Camera()
{
}
