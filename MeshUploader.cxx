#include "MeshUploader.h"
#include "Textures.hpp"

#include <cstring>
#include <stdexcept>
#include <vector>

/* ============================================================
   Helpers
   ============================================================ */

uint32_t findMemoryType(
    VkPhysicalDevice physicalDevice,
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(
        physicalDevice,
        &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

VkCommandBuffer beginSingleTimeCommands(
    VkDevice device,
    VkCommandPool commandPool)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
}

void endSingleTimeCommands(
    VkDevice device,
    VkQueue graphicsQueue,
    VkCommandPool commandPool,
    VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

/* ============================================================
   MeshUploader
   ============================================================ */

VkCommandBuffer MeshUploader::beginBatch(
    VkDevice device,
    VkCommandPool commandPool)
{
    return beginSingleTimeCommands(device, commandPool);
}

void MeshUploader::recordUpload(
    const VulkanContext &vkContext,
    GameMeshObject &mesh,
    VkCommandBuffer cmd,
    std::vector<PendingUpload> &garbage)
{
    mesh.vkContext = vkContext;

    /* ===================== VERTEX BUFFER ===================== */

    VkDeviceSize vertexSize =
        sizeof(mesh.vertices[0]) * mesh.vertices.size();

    VkBuffer vertexStaging;
    VkDeviceMemory vertexStagingMemory;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = vertexSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(
        vkContext.device,
        &bufferInfo,
        nullptr,
        &vertexStaging);

    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(
        vkContext.device,
        vertexStaging,
        &memReq);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReq.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        vkContext.physicalDevice,
        memReq.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(
        vkContext.device,
        &allocInfo,
        nullptr,
        &vertexStagingMemory);

    vkBindBufferMemory(
        vkContext.device,
        vertexStaging,
        vertexStagingMemory,
        0);

    void *data;
    vkMapMemory(
        vkContext.device,
        vertexStagingMemory,
        0,
        vertexSize,
        0,
        &data);

    memcpy(data, mesh.vertices.data(), (size_t)vertexSize);
    vkUnmapMemory(vkContext.device, vertexStagingMemory);

    bufferInfo.usage =
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    vkCreateBuffer(
        vkContext.device,
        &bufferInfo,
        nullptr,
        &mesh.vertexBuffer);

    vkGetBufferMemoryRequirements(
        vkContext.device,
        mesh.vertexBuffer,
        &memReq);

    allocInfo.allocationSize = memReq.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        vkContext.physicalDevice,
        memReq.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    vkAllocateMemory(
        vkContext.device,
        &allocInfo,
        nullptr,
        &mesh.vertexBufferMemory);

    vkBindBufferMemory(
        vkContext.device,
        mesh.vertexBuffer,
        mesh.vertexBufferMemory,
        0);

    VkBufferCopy vertexCopy{};
    vertexCopy.size = vertexSize;

    vkCmdCopyBuffer(
        cmd,
        vertexStaging,
        mesh.vertexBuffer,
        1,
        &vertexCopy);

    garbage.push_back({vertexStaging, vertexStagingMemory});

    /* ===================== INDEX BUFFER ===================== */

    if (!mesh.indices.empty())
    {
        VkDeviceSize indexSize =
            sizeof(mesh.indices[0]) * mesh.indices.size();

        VkBuffer indexStaging;
        VkDeviceMemory indexStagingMemory;

        bufferInfo.size = indexSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

        vkCreateBuffer(
            vkContext.device,
            &bufferInfo,
            nullptr,
            &indexStaging);

        vkGetBufferMemoryRequirements(
            vkContext.device,
            indexStaging,
            &memReq);

        allocInfo.allocationSize = memReq.size;
        allocInfo.memoryTypeIndex = findMemoryType(
            vkContext.physicalDevice,
            memReq.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        vkAllocateMemory(
            vkContext.device,
            &allocInfo,
            nullptr,
            &indexStagingMemory);

        vkBindBufferMemory(
            vkContext.device,
            indexStaging,
            indexStagingMemory,
            0);

        vkMapMemory(
            vkContext.device,
            indexStagingMemory,
            0,
            indexSize,
            0,
            &data);

        memcpy(data, mesh.indices.data(), (size_t)indexSize);
        vkUnmapMemory(vkContext.device, indexStagingMemory);

        bufferInfo.usage =
            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        vkCreateBuffer(
            vkContext.device,
            &bufferInfo,
            nullptr,
            &mesh.indexBuffer);

        vkGetBufferMemoryRequirements(
            vkContext.device,
            mesh.indexBuffer,
            &memReq);

        allocInfo.allocationSize = memReq.size;
        allocInfo.memoryTypeIndex = findMemoryType(
            vkContext.physicalDevice,
            memReq.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        vkAllocateMemory(
            vkContext.device,
            &allocInfo,
            nullptr,
            &mesh.indexBufferMemory);

        vkBindBufferMemory(
            vkContext.device,
            mesh.indexBuffer,
            mesh.indexBufferMemory,
            0);

        VkBufferCopy indexCopy{};
        indexCopy.size = indexSize;

        vkCmdCopyBuffer(
            cmd,
            indexStaging,
            mesh.indexBuffer,
            1,
            &indexCopy);

        garbage.push_back({indexStaging, indexStagingMemory});
    }
}

void MeshUploader::endBatch(
    const VulkanContext &vkContext,
    VkCommandBuffer cmd,
    const std::vector<PendingUpload> &garbage)
{
    endSingleTimeCommands(
        vkContext.device,
        vkContext.graphicsQueue,
        vkContext.commandPool,
        cmd);

    for (const auto &g : garbage)
    {
        vkDestroyBuffer(vkContext.device, g.buffer, nullptr);
        vkFreeMemory(vkContext.device, g.memory, nullptr);
    }
}
