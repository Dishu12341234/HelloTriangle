#include "Terrain.h"
#include "models/StandarBoxModel.h"
#include <cmath>
#include <iomanip>

Terrain::Terrain(VulkanContext vkContext, GameObjectPool &gop) : gameObjectPool{gop}
{
    this->vkContext = vkContext;
}

void Terrain::generateChunks()
{
    int xn = 20;
    int yn = 20;
    for (int x = 0; x < xn; x++)
    {
        for (int y = 0; y < yn; y++)
        {
            StandardBoxModel *sbm = new StandardBoxModel({0, 1, 1, 1, 1, 1}, vkContext);
            
            sbm->transform.position = glm::vec3(float(x) / 10, float(y) / 10, 0);
            sbm->tileIndex = 0;

            gameObjectPool.appendGameObject(sbm);
        }
    }

    // for (size_t i = 0; i < 4; i++)
    // {
    //     chunks.emplace_back(gameObjectPool, vkContext);

    //     Chunk &chunk = chunks.back();

    //     chunk.offset.x = i % 2;
    //     chunk.offset.y = i / 2;
    //     chunk.chunkId = chunkid++;
    //     chunk.generateChunk();
    // }
}
// TODO : make chunks store ptrs

StandardBoxModel* Terrain::getBlock(int xBlock, int yBlock, int zBlock)
{

    if (zBlock > 255)
        return nullptr;

    int xChunk = xBlock >> 4;
    int yChunk = yBlock >> 4;

    int xCBlock = xBlock & 15;
    int yCBlock = yBlock & 15;

    // std::cout << "Chunk: " << "(" << xChunk << "," << yChunk << ")" << "\n";
    // std::cout << "chunk local block: " << "(" << xCBlock << "," << yCBlock << ")" << "\n";

    for (size_t i = 0; i < chunks.size(); i++)
    {
        if (chunks[i].offset.x == xChunk && chunks[i].offset.y == yChunk)
        {
            std::cout << "Chunk id: " << chunks[i].chunkId << std::endl;
            // std::cout << "Chunk x: " << chunks[i].offset.x << std::endl;
            // std::cout << "Chunk y: " << chunks[i].offset.y << std::endl;
            // std::cout << "my x: " << xChunk << std::endl;
            // std::cout << "my y: " << yChunk << std::endl;
            for (size_t z = 0; z < 64; z++)
            {
                if (floor(chunks[i].layers[z].z) == floor(zBlock))
                {
                    std::cout << "Layer " << chunks[i].layers[z].z << std::endl;
                    return chunks[i].layers[z].blocks[xCBlock][yCBlock];
                }
                else
                {
                    std::cout << std::setprecision(20) << chunks[i].layers[z].z << std::endl;
                    std::cout << std::setprecision(20) << zBlock << std::endl;
                    std::cout << "condtiton " << (zBlock == chunks[i].layers[z].z) << std::endl;
                }
            }
        }
    }

    return nullptr;
}

Terrain::~Terrain()
{
}
