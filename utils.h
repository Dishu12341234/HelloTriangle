#ifndef UTILS_H
#define UTILS_H
#include "PassInfo.hpp"

namespace utils
{
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propeties, VulkanContext &context);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory, VulkanContext &vkContext);

} // namespace utils

#endif