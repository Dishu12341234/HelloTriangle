#ifndef STANDART_BOX_MODEL_H
#define STANDART_BOX_MODEL_H

#include "../GameObject.h"
#include <array>

#define ATLAS_SIZE 320.0f
#define TILES_PER_ROW 20
#define TILE_SIZE 16.0f
#define U 0.05f
#define GRASS_UVS {0, 1, 1, 1, 1, 1}
#define WOOD_OAK_UVS {3, 3, 3, 3, 3, 3}
#define LEAF_OAK_UVS {4, 4, 4, 4, 4, 4}

enum Face
{
    FRONT = 0,
    BACK = 1,
    LEFT = 2,
    RIGHT = 3,
    TOP = 4,
    BOTTOM = 5
};

class StandardBoxModel : public GameObject
{
private:
    float X_POSITIVE_UVIndexOffset{0};
    float X_NEGATIVE_UVIndexOffset{0};
    float Y_POSITIVE_UVIndexOffset{0};
    float Y_NEGATIVE_UVIndexOffset{0};
    float Z_POSITIVE_UVIndexOffset{0};
    float Z_NEGATIVE_UVIndexOffset{0};
    VulkanContext vkContext;

public:
    StandardBoxModel(std::array<float, 6> faceUVTextureOffsets, VulkanContext vkContext);
    void cleanUpResources() override;
    static glm::vec2 tileIndexToUV(uint32_t tileIndex);
    ~StandardBoxModel();
};

#endif