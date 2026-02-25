#include "Chunk.h"

#include <cmath>

// Simple hash-based noise (no external library needed)
float noise2D(int x, int y, int seed = 42)
{
    int n = x + y * 57 + seed * 131;
    n = (n << 13) ^ n;
    return 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
}

float smoothNoise(float x, float y)
{
    int ix = (int)x, iy = (int)y;
    float fx = x - ix, fy = y - iy;

    // Smoothstep
    float ux = fx * fx * (3 - 2 * fx);
    float uy = fy * fy * (3 - 2 * fy);

    float v1 = noise2D(ix, iy);
    float v2 = noise2D(ix + 1, iy);
    float v3 = noise2D(ix, iy + 1);
    float v4 = noise2D(ix + 1, iy + 1);

    return v1 + ux * (v2 - v1) + uy * (v3 - v1) + ux * uy * (v1 - v2 - v3 + v4);
}

// Octave noise for more natural terrain
float terrainHeight(float worldX, float worldY)
{
    float value = 0;
    float amplitude = 1.0f;
    float frequency = 0.05f;
    float max = 0;

    for (int o = 0; o < 4; o++)
    {
        value += smoothNoise(worldX * frequency, worldY * frequency) * amplitude;
        max += amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }

    value /= max;                 // normalize to -1..1
    return (value + 1.0f) / 2.0f; // remap to 0..1
}

Chunk::Chunk(GameObjectPool *GOP) : gameObjectPool{GOP}
{
    if (!GOP)
        throw std::runtime_error("Invalid GameObjectPool for Chunk Creation");
}

Chunk::~Chunk()
{
}

void Chunk::setOffset(glm::vec2 chunkOffset)
{
    this->chunkOffset = chunkOffset;
}

void Chunk::generateChunks()
{
    for (size_t k = 0; k < LAYER_COUNT; k++)
    {
        // if ((k > 14)&&(k < 16))
        //     for (size_t i = 0; i < 16; i++)
        //     {
        //         for (size_t j = 0; j < 16; j++)
        //         {
        //             BlockCoord coord{i + 16 * chunkOffset.x, j + 16 * chunkOffset.y, k};
        //             uint64_t blockID = gameObjectPool->createNewBoxModelAndAppend({coord.x, coord.y, coord.z}, (k > 10)
        //                                                                                                            ? std::vector<uint32_t>{0, 1, 1, 1, 1, 1}
        //                                                                                                            : std::vector<uint32_t>{5, 5, 5, 5, 5, 5})
        //                                    ->getID();

        //             // Emplace (won’t overwrite existing)
        //             auto [it, inserted] = blocks.emplace(coord, blockID);
        //         }
        //     }
        if (k >= 16 && k < 20)
        {
            for (size_t i = 0; i < 16; i++)
            {
                for (size_t j = 0; j < 16; j++)
                {
                    int worldX = i + 16 * chunkOffset.x;
                    int worldY = j + 16 * chunkOffset.y;

                    // Simple pseudo height function
                    float height =
                        18.0f +
                        2.0f * sin(worldX * 0.15f) +
                        2.0f * cos(worldY * 0.15f);

                    if (k <= height)
                    {
                        BlockCoord coord{worldX, worldY, k};

                        uint64_t blockID =
                            gameObjectPool
                                ->createNewBoxModelAndAppend(
                                    {coord.x, coord.y, coord.z},
                                    {0, 1, 1, 1, 1, 1})
                                ->getID();

                        blocks.emplace(coord, blockID);
                    }
                }
            }
        }
    }

    // for (int k = 0; k < LAYER_COUNT; k++)
    // {
    //     if (k < 32)
    //         for (int i = 0; i < 16; i++)
    //         {
    //             for (int j = 0; j < 16; j++)
    //             {
    //                 cullHiddenFaces(getBlockFromCoords(BlockCoord{i + int(16 * chunkOffset.x), j + int(16 * chunkOffset.y), k}, *this, *gameObjectPool));
    //             }
    //         }
    // }
}