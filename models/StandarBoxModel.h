#ifndef STANDART_BOX_MODEL_H
#define STANDART_BOX_MODEL_H

#include "../GameObject.h"
#include <array>

#define ATLAS_SIZE 320.0f
#define TILES_PER_ROW 20
#define TILE_SIZE 16.0f
#define U 0.05f
#define GRASS_UVS {1, 1, 1, 1, 1, 1}
#define WOOD_OAK_UVS {3, 3, 3, 3, 3, 3}
#define LEAF_OAK_UVS {4, 4, 4, 4, 4, 4}

#define AIR 0
#define GRASS 1
#define DIRT 2
#define STONE 3
#define SAND 4

// vkContext can be romoved

class StandardBoxModel : public GameObject
{
private:
    VulkanContext vkContext;
    uint8_t blockID{UINT8_MAX};   
    
    std::array<bool, 6> faces;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    friend class Chunk;
    friend class Camera;

    uint16_t blockType = AIR;

public:
    StandardBoxModel(std::vector<uint32_t> faceUVTextureOffsets, VulkanContext vkContext);
    void cleanUpResources() override;
    static glm::vec2 tileIndexToUV(uint32_t tileIndex);
    void removeFace(Face face);
    void addFace(Face face, int atlasIndex);
    ~StandardBoxModel();
};

#endif