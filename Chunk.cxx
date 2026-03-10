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
        if ((k >= 0) && (k < 50))
            for (size_t i = 0; i < 16; i++)
            {
                for (size_t j = 0; j < 16; j++)
                {
                    BlockCoord coord{i + 16 * chunkOffset.x, j + 16 * chunkOffset.y, k};
                    auto block = gameObjectPool->createNewBoxModelAndAppend({coord.x, coord.y, coord.z}, (k > 50)
                                                                                                             ? std::vector<uint32_t>{0, 1, 1, 1, 1, 1}
                                                                                                             : std::vector<uint32_t>{5, 5, 5, 5, 5, 5});
                    uint64_t blockID = block->getID();
                    if (k > 50)
                        block->blockType = GRASS;
                    else
                        block->blockType = STONE;

                    // Emplace (won’t overwrite existing)
                    auto [it, inserted] = blocks.emplace(coord, blockID);
                }
            }
        if (k >= 50 && k < 64)
        {
            for (size_t i = 0; i < 16; i++)
            {
                for (size_t j = 0; j < 16; j++)
                {
                    int worldX = i + 16 * chunkOffset.x;
                    int worldY = j + 16 * chunkOffset.y;

                    // Simple pseudo height function
                    float height =
                        64 +
                        2.0f * sin(worldX * 0.15f) +
                        2.0f * cos(worldY * 0.15f);

                    // Cavity center in world space
                    float caveCenterX = 16 * chunkOffset.x + 8; // center of chunk in X
                    float caveCenterY = 16 * chunkOffset.y + 8; // center of chunk in Y
                    float caveCenterZ = 58;                     // somewhere inside terrain

                    float caveRadius = 5.0f;

                    // Distance from cave center
                    float dx = worldX - caveCenterX;
                    float dy = worldY - caveCenterY;
                    float dz = k - caveCenterZ;

                    float distSq = dx * dx + dy * dy + dz * dz;

                    // If inside sphere → skip block creation (carve cavity)
                    if (distSq < caveRadius * caveRadius)
                        continue;

                    if (k <= height)
                    {
                        // ---- CAVITY CARVING ----
                        float caveCenterX = 16 * chunkOffset.x + 8;
                        float caveCenterY = 16 * chunkOffset.y + 8;
                        float caveCenterZ = 50;
                        float caveRadius = 5.0f;

                        float dx = worldX - caveCenterX;
                        float dy = worldY - caveCenterY;
                        float dz = k - caveCenterZ;

                        if (dx * dx + dy * dy + dz * dz < caveRadius * caveRadius)
                            continue;
                        // -------------------------

                        BlockCoord coord{worldX, worldY, k};
                        auto block =
                            gameObjectPool
                                ->createNewBoxModelAndAppend(
                                    {coord.x, coord.y, coord.z},
                                    {0, 1, 1, 1, 1, 1});
                        block->blockType = GRASS;
                        uint64_t blockID =
                            block->getID();

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

void Chunk::generateChunkMesh()
{
    chunkMeshObject = gameObjectPool->createNewGameObject();

    std::vector<Vertex> vertices;
    vertices.reserve(100000);
    // +Y TOP
    float tileSize = 0.1f;
    int N = 16;
    int n = 0;

    for (size_t z = 0; z < 256; z++)
    {
        for (int x = 0; x < N; x++)
        {
            for (int y = 0; y < N; y++)
            {
                float fx = x * tileSize + chunkOffset.x * N * tileSize - tileSize / 2;
                float fy = y * tileSize + chunkOffset.y * N * tileSize - tileSize / 2;

                float zHeight = 0.0f;
                auto it = blocks.find({x + chunkOffset.x * N, y + chunkOffset.y * N, z});
                if (it == blocks.end())
                    continue;
                StandardBoxModel *block = gameObjectPool->getBlock(it->second);

                if (!block)
                    continue;

                if (!block->mesh)
                    continue;

                if ((block->mesh->indices.size() == 0) || (block->mesh->vertices.size() == 0) || block->blockType == AIR)
                    continue;

                zHeight = z * tileSize + .4226;

                if (block->faces[TOP])
                {
                    chunkMeshObject->faceUVTextureOffsets[TOP] = block->faceUVTextureOffsets[TOP];
                    vertices.emplace_back(glm::vec3{fx, fy, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[TOP], 0);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[TOP], 1);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy + tileSize, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[TOP], 2);
                    vertices.emplace_back(glm::vec3{fx, fy + tileSize, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[TOP], 3);
                    n++;
                }

                if (block->faces[BOTTOM])
                {
                    chunkMeshObject->faceUVTextureOffsets[BOTTOM] = block->faceUVTextureOffsets[BOTTOM];

                    vertices.emplace_back(glm::vec3{fx, fy, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[BOTTOM], 0);
                    vertices.emplace_back(glm::vec3{fx, fy + tileSize, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[BOTTOM], 1);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy + tileSize, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[BOTTOM], 2);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[BOTTOM], 3);
                    n++;
                }

                if (block->faces[LEFT])
                {
                    chunkMeshObject->faceUVTextureOffsets[LEFT] = block->faceUVTextureOffsets[LEFT];

                    vertices.emplace_back(glm::vec3{fx + tileSize, fy, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[LEFT], 0);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy + tileSize, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[LEFT], 1);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy + tileSize, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[LEFT], 2);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[LEFT], 3);
                    n++;
                }

                if (block->faces[RIGHT])
                {
                    chunkMeshObject->faceUVTextureOffsets[RIGHT] = block->faceUVTextureOffsets[RIGHT];
                    vertices.emplace_back(glm::vec3{fx, fy, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[RIGHT], 0);
                    vertices.emplace_back(glm::vec3{fx, fy, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[RIGHT], 1);
                    vertices.emplace_back(glm::vec3{fx, fy + tileSize, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[RIGHT], 2);
                    vertices.emplace_back(glm::vec3{fx, fy + tileSize, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[RIGHT], 3);
                    n++;
                }

                if (block->faces[FRONT])
                {
                    chunkMeshObject->faceUVTextureOffsets[FRONT] = block->faceUVTextureOffsets[FRONT];

                    vertices.emplace_back(glm::vec3{fx, fy + tileSize, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[FRONT], 0);
                    vertices.emplace_back(glm::vec3{fx, fy + tileSize, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[FRONT], 1);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy + tileSize, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[FRONT], 2);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy + tileSize, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[FRONT], 3);
                    n++;
                }

                if (block->faces[BACK])
                {
                    chunkMeshObject->faceUVTextureOffsets[BACK] = block->faceUVTextureOffsets[BACK];
                    vertices.emplace_back(glm::vec3{fx, fy, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[BACK], 0);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy, zHeight - tileSize}, glm::vec3{tileSize}, block->faceUVTextureOffsets[BACK], 1);
                    vertices.emplace_back(glm::vec3{fx + tileSize, fy, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[BACK], 2);
                    vertices.emplace_back(glm::vec3{fx, fy, zHeight}, glm::vec3{tileSize}, block->faceUVTextureOffsets[BACK], 3);
                    n++;
                }
            }
        }
    }

    std::vector<uint32_t> indices;
    indices.reserve(150000);
    std::cout << "n: " << n << std::endl;
    for (size_t i = 0; i < n; i++)
    {
        indices.push_back(4 * i);
        indices.push_back(4 * i + 1);
        indices.push_back(4 * i + 2);
        indices.push_back(4 * i + 2);
        indices.push_back(4 * i + 3);
        indices.push_back(4 * i);
    }

    chunkMeshObject->loadGeometry(vertices, indices);
    chunkMeshObject->transform.position.y = 0;
    chunkMeshObject->transform.position.x = 0;

    gameObjectPool->appendGameObject(chunkMeshObject);
    std::cout << "Chunk (" << chunkOffset.x << "," << chunkOffset.y
              << ") vertices: " << vertices.size()
              << " triangles: " << indices.size() / 3 << std::endl;
}

// for (z = 255; z >= -1; z--)
//             {
//                 auto it = blocks.find({x + chunkOffset.x * N, y + chunkOffset.y * N, z});
//                 if (it != blocks.end())
//                 {
//                     zHeight = z * tileSize + .4226;
//                     chunkMeshObject->faceUVTextureOffsets[0] = gameObjectPool->getBlock(it->second)->faceUVTextureOffsets[TOP];
//                     break;
//                 }
//             }

//             if (z > 0)
//             {
//                 vertices.emplace_back(glm::vec3{fx, fy, zHeight}, glm::vec3{tileSize}, glm::vec2{0, 0}, 0);
//                 vertices.emplace_back(glm::vec3{fx + tileSize, fy, zHeight}, glm::vec3{tileSize},  1);
//                 vertices.emplace_back(glm::vec3{fx + tileSize, fy + tileSize, zHeight}, glm::vec3{tileSize}, glm::vec2{1, 1}, 2);
//                 vertices.emplace_back(glm::vec3{fx, fy + tileSize, zHeight}, glm::vec3{tileSize}, glm::vec2{0, 1}, 3);
//             }