#include "Camera.h"
#include "HelloTriangleApplication.hpp"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const glm::vec3 &v)
{
    os << "vec3{x=" << v.x << ", y=" << v.y << ", z=" << v.z << "}";
    return os;
}

bool Camera::isVoxelSolid(int x, int y, int z)
{
    auto block = gameObjectPool.getBlockFromWorldCoords({x, y, z});
    if (block && block->objectType == ObjectType::StandardBoxModel && block->blockType != AIR)
        return true;
    return false;
}

Camera::Camera(VulkanContext vkContext, GameObjectPool &gop)
    : gameObjectPool{gop}, rayCamera{vkContext, gameObjectPool}
{
    // rays.emplace_back(gop);

    this->vkContext = vkContext;

    ray = gameObjectPool.createNewGameObject();
    gameObjectPool.appendGameObject(ray);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // Unit ray mesh along Z (0 → 1), scaled at draw time
    vertices.emplace_back(glm::vec3{0, 0, 0}, glm::vec3{1, 0, 0}, TOP, 0);
    vertices.emplace_back(glm::vec3{0, 0, 1}, glm::vec3{0, 1, 0}, TOP, 1);

    indices.push_back(0);
    indices.push_back(1);

    ray->loadGeometry(vertices, indices);
    ray->vkContext = vkContext;
}

void Camera::updateUBO(UniformBufferObject &UBO,
                       VkExtent2D &swapChainExtent,
                       Event &event)
{
    static double lastX = event.mouseX;
    static double lastY = event.mouseY;

    float dt = event.dt * 0.001f;

    double dx = lastX - event.mouseX;
    double dy = lastY - event.mouseY;

    lastX = event.mouseX;
    lastY = event.mouseY;

    float sensitivity = 0.1f;

    yaw += dx * sensitivity;
    pitch += dy * sensitivity;
    pitch = glm::clamp(pitch, -89.9f, 89.9f);

    // Recompute forward from yaw/pitch
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.z = sin(glm::radians(pitch));
    forward = glm::normalize(forward);

    glm::vec3 worldUp(0, 0, 1);

    glm::vec3 forwardFlat = glm::normalize(glm::vec3(forward.x, forward.y, 0));
    glm::vec3 right = glm::normalize(glm::cross(forwardFlat, worldUp));

    float speed = 1.5f * dt;

    if (event.getKeyPressed(GLFW_KEY_W))
        cameraPos += forwardFlat * speed;
    if (event.getKeyPressed(GLFW_KEY_S))
        cameraPos -= forwardFlat * speed;
    if (event.getKeyPressed(GLFW_KEY_A))
        cameraPos -= right * speed;
    if (event.getKeyPressed(GLFW_KEY_D))
        cameraPos += right * speed;
    if (event.getKeyPressed(GLFW_KEY_SPACE))
        cameraPos.z += speed;
    if (event.getKeyPressed(GLFW_KEY_LEFT_SHIFT))
        cameraPos.z -= speed;

    UBO.view = glm::lookAt(cameraPos, cameraPos + forward, worldUp);

    UBO.proj = glm::perspective(
        glm::radians(45.0f),
        swapChainExtent.width / (float)swapChainExtent.height,
        0.1f,
        100.0f);

    UBO.proj[1][1] *= -1;
}

// Camera space: 1 unit = 10 voxels
glm::vec3 Camera::gePositionInWorldCoords() { return cameraPos * 10.f; }

void Camera::drawRays(VkCommandBuffer &commandBuffer,
                      std::vector<VkDescriptorSet> &descriptorSets,
                      u_GraphicsPipeline &graphicsPipeline,
                      VkExtent2D &swapChainExtent,
                      uint64_t instanceCount,
                      uint32_t &currentFrame)
{
    glm::vec3 rayOriginCam = cameraPos + forward * 0.1f;
    rayCamera.drawRay(glm::vec3({2, 2, 7.5f}), 20, forward, true, commandBuffer, descriptorSets, graphicsPipeline, swapChainExtent, instanceCount, currentFrame);

    // for (auto &&ray : rays)
    // {
    //     ray.drawRay(cameraPos, forward, commandBuffer, descriptorSets, graphicsPipeline, swapChainExtent, instanceCount, currentFrame);
    // }
}

Camera::~Camera() {}

Ray::Ray(VulkanContext vkContext, GameObjectPool &gop) : gameObjectPool{gop}
{
    ray = gameObjectPool.createNewGameObject();
    gameObjectPool.appendGameObject(ray);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // Unit ray mesh along Z (0 → 1), scaled at draw time
    vertices.emplace_back(glm::vec3{0, 0, 0}, glm::vec3{1, 0, 0}, TOP, 0);
    vertices.emplace_back(glm::vec3{0, 0, 1}, glm::vec3{0, 1, 0}, TOP, 1);

    indices.push_back(0);
    indices.push_back(1);

    ray->loadGeometry(vertices, indices);
    ray->vkContext = vkContext;
}

void Ray::drawRay(glm::vec3 rayOrigin, int rayLength, glm::vec3 forward, bool followCameraDir, VkCommandBuffer &commandBuffer,
                  std::vector<VkDescriptorSet> &descriptorSets,
                  u_GraphicsPipeline &graphicsPipeline,
                  VkExtent2D &swapChainExtent,
                  uint64_t instanceCount,
                  uint32_t &currentFrame)
{
    // ---------------------------------------------------------------
    // Camera space origin (used for rendering — matches view matrix)
    // ---------------------------------------------------------------
    glm::vec3 rayDir = glm::normalize(forward);

    // ---------------------------------------------------------------
    // Voxel space origin (1 voxel = 1 unit, used for DDA lookup)
    // ---------------------------------------------------------------
    glm::vec3 rayOriginVox = rayOrigin * 10.f;

    const float maxDistVox = rayLength; // max reach in voxel units

    // Starting voxel
    glm::ivec3 voxel = glm::ivec3(glm::floor(rayOriginVox));

    // How far along the ray we must travel to cross one voxel in each axis
    glm::vec3 deltaDist = glm::abs(glm::vec3(
        1.0f / rayDir.x,
        1.0f / rayDir.y,
        1.0f / rayDir.z));

    glm::ivec3 step;
    glm::vec3 sideDist;

    // Initialise step direction and initial side distances
    if (rayDir.x < 0)
    {
        step.x = -1;
        sideDist.x = (rayOriginVox.x - voxel.x) * deltaDist.x;
    }
    else
    {
        step.x = 1;
        sideDist.x = (voxel.x + 1.0f - rayOriginVox.x) * deltaDist.x;
    }

    if (rayDir.y < 0)
    {
        step.y = -1;
        sideDist.y = (rayOriginVox.y - voxel.y) * deltaDist.y;
    }
    else
    {
        step.y = 1;
        sideDist.y = (voxel.y + 1.0f - rayOriginVox.y) * deltaDist.y;
    }

    if (rayDir.z < 0)
    {
        step.z = -1;
        sideDist.z = (rayOriginVox.z - voxel.z) * deltaDist.z;
    }
    else
    {
        step.z = 1;
        sideDist.z = (voxel.z + 1.0f - rayOriginVox.z) * deltaDist.z;
    }

    // ---------------------------------------------------------------
    // Check the starting voxel before stepping
    // ---------------------------------------------------------------
    bool hit = false;
    float distanceVox = 0.0f;

    if (isVoxelSolid(voxel.x, voxel.y, voxel.z))
    {
        hit = true;
        auto vox = gameObjectPool.getBlockFromWorldCoords({voxel.x, voxel.y, voxel.z});
        std::cout << "Hit voxel: " << voxel.x << " " << voxel.y << " " << voxel.z
                  << " (dist: " << distanceVox << " vox / " << distanceVox / 10.f << " cam)\n";

        if (vox)
            std::cout << "Block type : " << vox->blockType << std::endl;
    }

    // ---------------------------------------------------------------
    // DDA traversal
    // ---------------------------------------------------------------
    for (int i = 0; i < 200 && !hit; i++)
    {
        // Step along the axis whose boundary is closest
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z)
        {
            distanceVox = sideDist.x;
            sideDist.x += deltaDist.x;
            voxel.x += step.x;
        }
        else if (sideDist.y < sideDist.z)
        {
            distanceVox = sideDist.y;
            sideDist.y += deltaDist.y;
            voxel.y += step.y;
        }
        else
        {
            distanceVox = sideDist.z;
            sideDist.z += deltaDist.z;
            voxel.z += step.z;
        }

        if (distanceVox > maxDistVox)
            break;

        if (isVoxelSolid(voxel.x, voxel.y, voxel.z))
        {
            hit = true;
            auto vox = gameObjectPool.getBlockFromWorldCoords({voxel.x, voxel.y, voxel.z});
            if (!vox)
                continue;
            std::cout << "Hit voxel: " << voxel.x << " " << voxel.y << " " << voxel.z
                      << " (dist: " << distanceVox << " vox / " << distanceVox / 10.f << " cam)\n";

            std::cout << "Block type : " << vox->blockType << std::endl;
        }
    }

    // ---------------------------------------------------------------
    // Convert hit distance from voxel space back to camera space
    // ---------------------------------------------------------------
    float rayLengthCam = (hit ? distanceVox : maxDistVox) / 10.f;

    // ---------------------------------------------------------------
    // Build model matrix in camera space
    // ---------------------------------------------------------------

    if (fabs(glm::dot(rayDir, worldUp)) > 0.99f)
        worldUp = glm::vec3(0, 1, 0);

    glm::vec3 right = glm::normalize(glm::cross(worldUp, rayDir));
    glm::vec3 newUp = glm::cross(rayDir, right);

    glm::mat4 rotation(1.0f);
    if (followCameraDir)
    {
        rotation[0] = glm::vec4(right, 0);
        rotation[1] = glm::vec4(newUp, 0);
        rotation[2] = glm::vec4(rayDir, 0);
    }
    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), rayOrigin) *
        rotation *
        glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, rayLengthCam));

    auto mesh = ray->mesh;
    if (!mesh)
        return;

    VkBuffer vertexBuffers[] = {mesh->vertexBuffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        graphicsPipeline.pipelineLayout,
        0, 1,
        &(descriptorSets[currentFrame]),
        0, nullptr);

    PushConstantC1 c1;
    c1.model = model;

    vkCmdPushConstants(
        commandBuffer,
        graphicsPipeline.pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0, sizeof(PushConstantC1),
        &c1);

    vkCmdDrawIndexed(
        commandBuffer,
        static_cast<uint32_t>(mesh->indices.size()),
        1, 0, 0, 0);
}

bool Ray::isVoxelSolid(int x, int y, int z)
{
    auto block = gameObjectPool.getBlockFromWorldCoords({x, y, z});
    if (block && block->objectType == ObjectType::StandardBoxModel && block->blockType != AIR)
        return true;
    return false;
}
