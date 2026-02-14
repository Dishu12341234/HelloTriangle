#include "models/StandarBoxModel.h"

StandardBoxModel::StandardBoxModel(std::vector<uint32_t> faceUVTextureOffsetsa, VulkanContext vkContext) : GameObject{vkContext}
{
    this->vkContext = vkContext;
    faceUVTextureOffsets[0] = faceUVTextureOffsetsa[0];
    faceUVTextureOffsets[1] = faceUVTextureOffsetsa[1];
    faceUVTextureOffsets[2] = faceUVTextureOffsetsa[2];
    faceUVTextureOffsets[3] = faceUVTextureOffsetsa[3];
    faceUVTextureOffsets[4] = faceUVTextureOffsetsa[4];
    faceUVTextureOffsets[5] = faceUVTextureOffsetsa[5];
    std::vector<Vertex> vertices = {

        // +Z FRONT (side → tile 1)
        {{-.05f, -.05f, .05f}, {.05f, .05f, .05f}, {0.f, 0.f}, 0},
        {{.05f, -.05f, .05f}, {.05f, .05f, .05f}, {1.f, 0.f}, 1},
        {{.05f, .05f, .05f}, {.05f, .05f, .05f}, {1.f, 1.f}, 2},
        {{-.05f, .05f, .05f}, {.05f, .05f, .05f}, {0.f, 1.f}, 3},

        // -Z BACK (side → tile 1)
        {{.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {0.f, 0.f}, 4},
        {{-.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {1.f, 0.f}, 5},
        {{-.05f, .05f, -.05f}, {.05f, .05f, .05f}, {1.f, 1.f}, 6},
        {{.05f, .05f, -.05f}, {.05f, .05f, .05f}, {0.f, 1.f}, 7},

        // -X LEFT (side → tile 1)
        {{-.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {0.f, 0.f}, 8},
        {{-.05f, -.05f, .05f}, {.05f, .05f, .05f}, {1.f, 0.f}, 9},
        {{-.05f, .05f, .05f}, {.05f, .05f, .05f}, {1.f, 1.f}, 10},
        {{-.05f, .05f, -.05f}, {.05f, .05f, .05f}, {0.f, 1.f}, 11},

        // +X RIGHT (side → tile 1)
        {{.05f, -.05f, .05f}, {.05f, .05f, .05f}, {0.f, 0.f}, 12},
        {{.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {1.f, 0.f}, 13},
        {{.05f, .05f, -.05f}, {.05f, .05f, .05f}, {1.f, 1.f}, 14},
        {{.05f, .05f, .05f}, {.05f, .05f, .05f}, {0.f, 1.f}, 15},

        // +Y FRONT (tile 0)
        {{-.05f, .05f, .05f}, {.05f, .05f, .05f}, {0.f, 0.f}, 16},
        {{.05f, .05f, .05f}, {.05f, .05f, .05f}, {1.f, 0.f}, 17},
        {{.05f, .05f, -.05f}, {.05f, .05f, .05f}, {1.f, 1.f}, 18},
        {{-.05f, .05f, -.05f}, {.05f, .05f, .05f}, {0.f, 1.f}, 19},

        // -Y BACK (tile 1)
        {{-.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {0.f, 0.f}, 20},
        {{.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {1.f, 0.f}, 21},
        {{.05f, -.05f, .05f}, {.05f, .05f, .05f}, {1.f, 1.f}, 22},
        {{-.05f, -.05f, .05f}, {.05f, .05f, .05f}, {0.f, 1.f}, 23},
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20};

    loadGeometry(vertices, indices);
}

void StandardBoxModel::cleanUpResources()
{
    GameObject::cleanUpResources();
}

glm::vec2 StandardBoxModel::tileIndexToUV(uint32_t tileIndex)
{
    float tileSize = TILE_SIZE / ATLAS_SIZE;

    uint32_t tileX = tileIndex % TILES_PER_ROW;
    uint32_t tileY = tileIndex / TILES_PER_ROW;

    return glm::vec2(
        tileX * tileSize,
        tileY * tileSize);
}
StandardBoxModel::~StandardBoxModel()
{
}
