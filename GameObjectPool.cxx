#include "GameObjectPool.h"
#include "Terrain.h"

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

void GameObjectPool::appendGameObject(GameObject *gameObject)
{
    this->gameObjects.push_back(gameObject);
    if (gameObject->objectType == ObjectType::StandardBoxModel)
    {
        return;
        if (!terrain)
            return;
        std::cout << "Reducing mesh" << std::endl;

        auto left = terrain->getBlock(int(gameObject->transform.position.x * 10) + 1, gameObject->transform.position.y, gameObject->transform.position.z);
        if (left)
        {
            StandardBoxModel::removeFace(gameObject, RIGHT);
            left->removeFace(LEFT);
        }

        auto right = terrain->getBlock(int(gameObject->transform.position.x * 10) - 1, gameObject->transform.position.y, gameObject->transform.position.z);
        if (right)
        {
            StandardBoxModel::removeFace(gameObject, LEFT);
            right->removeFace(RIGHT);
        }

        auto bottom = terrain->getBlock(gameObject->transform.position.x, gameObject->transform.position.y, int(gameObject->transform.position.z * 10) - 1);
        if (bottom)
        {
            StandardBoxModel::removeFace(gameObject, TOP);
            bottom->removeFace(BOTTOM);
        }

        auto top = terrain->getBlock(gameObject->transform.position.x, gameObject->transform.position.y, int(gameObject->transform.position.z * 10) + 1);
        if (top)
        {
            StandardBoxModel::removeFace(gameObject, BOTTOM);
            top->removeFace(TOP);
        }

        auto front = terrain->getBlock(gameObject->transform.position.x, int(gameObject->transform.position.y * 10) + 1, gameObject->transform.position.z);
        if (front)
        {
            StandardBoxModel::removeFace(gameObject, BACK);
            front->removeFace(FRONT);
        }

        auto back = terrain->getBlock(gameObject->transform.position.x, int(gameObject->transform.position.y * 10) - 1, gameObject->transform.position.z);
        if (back)
        {
            StandardBoxModel::removeFace(gameObject, FRONT);
            back->removeFace(BACK);
        }
    }
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
    for (auto &&gameObject : gameObjects)
    {
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

    for (auto& go : gameObjects)
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
