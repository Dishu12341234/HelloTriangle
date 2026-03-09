#include "utils.h"
#include <stdexcept>
#include <cstring>

uint32_t utils::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propeties, VulkanContext &context)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(context.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & propeties) == propeties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void utils::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory, VulkanContext &vkContext)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(vkContext.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkContext.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    uint32_t typeFilter = memRequirements.memoryTypeBits;

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkContext.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            allocInfo.memoryTypeIndex = i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");

    if (vkAllocateMemory(vkContext.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(vkContext.device, buffer, bufferMemory, 0);

    void *data;
    vkMapMemory(vkContext.device, bufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, nullptr, (size_t)bufferInfo.size);
    vkUnmapMemory(vkContext.device, bufferMemory);
}
