#include "GameObjectPool.h"

GameObjectPool::GameObjectPool()
{
}

void GameObjectPool::init(VulkanContext context)
{
    this->vkContext = context;
    uploadGarbage.reserve(256);
}

GameObject *GameObjectPool::createNewGameObject(std::string modelPath)
{
    GameObject *gameObject = new GameObject(vkContext);
    gameObject->loadGeometry(modelPath);
    return gameObject;
}

GameObject *GameObjectPool::createNewGameObject()
{
    GameObject *gameObject = new GameObject(vkContext);
    return gameObject;
}

void GameObjectPool::appendGameObject(GameObject *gameObject)
{
    this->gameObjects.push_back(gameObject);
}

void GameObjectPool::uploadVBOsAndIBOs()
{
    MeshUploader meshUploader;

    VkCommandBuffer uploadCmd =
        meshUploader.beginBatch(
            vkContext.device,
            vkContext.commandPool);

    for (auto &&gameObject : gameObjects)
    {
        meshUploader.recordUpload(vkContext, *gameObject->mesh, uploadCmd, uploadGarbage);
    }

    meshUploader.endBatch(vkContext, uploadCmd, uploadGarbage);
}

void GameObjectPool::drawIndexed(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame)
{
    // for (auto &&gameObject : gameObjects)
    // {
    //     gameObject->drawIndexed(commandBuffer, descriptorSets, graphicsPipeline, swapChainExtent, instanceCount, currentFrame);
    // }

    auto mesh = gameObjects[1]->mesh;

    if (!mesh)
        return;

    VkBuffer vertexBuffers[] = {mesh->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &(descriptorSets[currentFrame]), 0, nullptr);

    // PushConstantC1 c1;
    // c1.model = glm::translate(glm::mat4(1.f), glm::vec3(transform.position));

    // vkCmdPushConstants(commandBuffer, graphicsPipeline.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantC1), &c1);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh->indices.size()), gameObjects.size(), 0, 0, 0);
}

void GameObjectPool::cleanUpResources()
{
    for (auto &&gameObject : gameObjects)
    {
        gameObject->cleanUpResources();
        delete gameObject;
    }

    gameObjects.clear();
}

GameObjectPool::~GameObjectPool()
{
}
