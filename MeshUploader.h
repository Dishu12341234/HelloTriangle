#ifndef MESH_UPLOADER
#define MESH_UPLOADER

#include <vulkan/vulkan.h>
#include "GameMeshObject.h"

struct PendingUpload
{
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;
};

class MeshUploader
{
public:
    VkCommandBuffer beginBatch(
        VkDevice device,
        VkCommandPool commandPool);
    void recordUpload(
        const VulkanContext &vkContext,
        GameMeshObject &mesh,
        VkCommandBuffer cmd,
        std::vector<PendingUpload> &garbage);
    static void upload(
        VulkanContext &vkContext,
        GameMeshObject &mesh);
    void endBatch(const VulkanContext &vkContext,
                  VkCommandBuffer cmd,
                  const std::vector<PendingUpload> &garbage);
};

#endif