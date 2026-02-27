#include "GameObjectPool.h"

#include <thread>

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

StandardBoxModel *GameObjectPool::createNewBoxModelAndAppend(glm::vec3 blockCoord, std::vector<uint32_t> faceUVTextureOffsets)
{
    StandardBoxModel *model = new StandardBoxModel(faceUVTextureOffsets, vkContext);
    model->transform.position = blockCoord / 10.f;
    appendGameObject(model);

    return model;
}

StandardBoxModel *GameObjectPool::getBlock(uint64_t goid)
{
    auto it = gameObjects.find(goid);
    if (it != gameObjects.end())
    {
        if (it->second->objectType == ObjectType::StandardBoxModel)
            return static_cast<StandardBoxModel *>(it->second);
    }

    return nullptr;
}

void GameObjectPool::appendGameObject(GameObject *gameObject)
{
    this->gameObjects.insert({gameObject->getID(), gameObject});
}

void GameObjectPool::initUpload()
{
    uploadIndex = 0;
    uploadInProgress = true;

    uploadCmd = meshUploader.beginBatch(
        vkContext.device,
        vkContext.commandPool);
}

void GameObjectPool::uploadChunk()
{
    if (!uploadInProgress)
        return;

    const size_t MAX_UPLOADS_PER_FRAME = 8192; // tune this

    size_t total = gameObjects.size();
    size_t uploads = 0;

    while (uploadIndex < total && uploads < MAX_UPLOADS_PER_FRAME)
    {
        meshUploader.recordUpload(
            vkContext,
            *(gameObjects[uploadIndex]->mesh),
            uploadCmd,
            uploadGarbage);

        uploadIndex++;
        uploads++;
    }

    std::cout << "Uploaded " << 100.f * float(uploadIndex) / float(total) << "%" << std::endl;

    // If finished uploading everything
    if (uploadIndex >= total)
    {
        meshUploader.endBatch(
            vkContext,
            uploadCmd,
            uploadGarbage);

        uploadInProgress = false;
        ready = true;
    }
}

void GameObjectPool::drawIndexed(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t instanceCount, uint32_t &currentFrame)
{
    for (auto &&[goid, gameObject] : gameObjects)
    {
        if(gameObject->objectType == ObjectType::StandardBoxModel)
        gameObject->drawIndexed(commandBuffer, descriptorSets, graphicsPipeline, swapChainExtent, instanceCount, currentFrame);
    }

    // auto mesh = gameObjects[1]->mesh;

    // if (!mesh)
    //     return;

    // VkBuffer vertexBuffers[] = {mesh->vertexBuffer};
    // VkDeviceSize offsets[] = {0};
    // vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    // vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    // vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &(descriptorSets[currentFrame]), 0, nullptr);

    // // PushConstantC1 c1;
    // // c1.model = glm::translate(glm::mat4(1.f), glm::vec3(transform.position));

    // // vkCmdPushConstants(commandBuffer, graphicsPipeline.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantC1), &c1);

    // vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh->indices.size()), gameObjects.size(), 0, 0, 0);
}

void GameObjectPool::reuploadAll()
{
}

void GameObjectPool::cleanUpResources()
{
    vkDeviceWaitIdle(vkContext.device);

    for (auto &[goid, go] : gameObjects)
    {
        if (go->mesh)
            go->mesh->destroyGPU(vkContext.device);
        go->cleanUpResources();

        delete go;
    }

    gameObjects.clear();
}
GameObjectPool::~GameObjectPool()
{
}
