#ifndef MESH_UPLOADER
#define MESH_UPLOADER

#include <vulkan/vulkan.h>
#include "GameMeshObject.h"

struct PendingUpload
{
    VkBuffer buffer;
    VkDeviceMemory memory;
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

    void endBatch(const VulkanContext &vkContext,
                  VkCommandBuffer cmd,
                  const std::vector<PendingUpload> &garbage);
};

#endif