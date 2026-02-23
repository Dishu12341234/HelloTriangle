#include "Terrain.h"
#include "models/StandarBoxModel.h"
#include <cmath>
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#define TIME_START(name) \
    auto name##_start = std::chrono::high_resolution_clock::now();

#define TIME_END(name)                                                                        \
    auto name##_end = std::chrono::high_resolution_clock::now();                              \
    std::cout << #name << " took "                                                            \
              << std::chrono::duration<double, std::milli>(name##_end - name##_start).count() \
              << " ms\n";

Terrain::Terrain(VulkanContext vkContext, GameObjectPool &gop) : gameObjectPool{gop}
{
    this->vkContext = vkContext;
    this->gameObjectPool.terrain = this;
}

void Terrain::generateChunks()
{
    // int xn = 20;
    // int yn = 20;
    // for (int x = 0; x < xn; x++)
    // {
    //     for (int y = 0; y < yn; y++)
    //     {
    //         StandardBoxModel *sbm = new StandardBoxModel({0, 1, 1, 1, 1, 1}, vkContext);

    //         sbm->transform.position = glm::vec3(float(x) / 10, float(y) / 10, 0);
    //         sbm->tileIndex = 0;

    //         gameObjectPool.appendGameObject(sbm);
    //     }
    // }

    // for (size_t i = 0; i < 4; i++)
    // {
    int x = 0;
    int y = 0;

    auto key = std::make_pair(x, y);

    auto [it, inserted] = chunks.try_emplace(
        std::make_pair(x, y),
        gameObjectPool,
        vkContext);

    Chunk &chunk = it->second;

    chunk.offset.x = x;
    chunk.offset.y = y;
    chunk.chunkId = chunkid++;
    chunk.generateChunk();
    // }
}

Chunk *Terrain::getChunk(int x, int y)
{
    auto it = chunks.find({x, y});
    if (it == chunks.end())
        return nullptr;

    return &it->second;
}

void Terrain::appendBlockToTerrain(GameObject *gameObject)
{

    int xChunk = int(gameObject->transform.position.x * 10) >> 4;
    int yChunk = int(gameObject->transform.position.y * 10) >> 4;
    int zBlock = int(gameObject->transform.position.z * 10);
    auto tChunk = getChunk(xChunk, yChunk);
    int xCBlock = int(gameObject->transform.position.x * 10) & 15;
    int yCBlock = int(gameObject->transform.position.y * 10) & 15;

    if (!tChunk)
        return;

    tChunk->layers[zBlock].blocks[xCBlock][yCBlock] = static_cast<StandardBoxModel *>(gameObject);
    gameObjectPool.appendGameObject(gameObject);
}
// TODO : make chunks store ptrs

StandardBoxModel *Terrain::getBlock(int xBlock, int yBlock, int zBlock)
{

    // std::ofstream dump("dump.txt");
    // std::string buffer;

    int xChunk = xBlock >> 4;
    int yChunk = yBlock >> 4;

    int xCBlock = xBlock & 15;
    int yCBlock = yBlock & 15;

    auto it = chunks.find({xChunk, yChunk});

    if (it == chunks.end())
        return nullptr;

    Chunk &chunk = it->second;
    
    if (zBlock >= 255)
        return nullptr;

    return chunk.layers[zBlock].blocks[xCBlock][yCBlock];
}

Terrain::~Terrain()
{
}
