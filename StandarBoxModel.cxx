#include "models/StandarBoxModel.h"

StandardBoxModel::StandardBoxModel(std::vector<uint32_t> faceUVTextureOffsetsa, VulkanContext vkContext) : GameObject{vkContext}
{
    this->vkContext = vkContext;
    std::vector<Vertex> vertices = {

        // +Z FRONT (side → tile 1)
        {{-.05f, -.05f, .05f}, {.05f, .05f, .05f}, {0.f, 0.f}},
        {{.05f, -.05f, .05f}, {.05f, .05f, .05f}, {1.f, 0.f}},
        {{.05f, .05f, .05f}, {.05f, .05f, .05f}, {1.f, 1.f}},
        {{-.05f, .05f, .05f}, {.05f, .05f, .05f}, {0.f, 1.f}},

        // -Z BACK (side → tile 1)
        {{.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {0.f, 0.f}},
        {{-.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {1.f, 0.f}},
        {{-.05f, .05f, -.05f}, {.05f, .05f, .05f}, {1.f, 1.f}},
        {{.05f, .05f, -.05f}, {.05f, .05f, .05f}, {0.f, 1.f}},

        // -X LEFT (side → tile 1)
        {{-.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {0.f, 0.f}},
        {{-.05f, -.05f, .05f}, {.05f, .05f, .05f}, {1.f, 0.f}},
        {{-.05f, .05f, .05f}, {.05f, .05f, .05f}, {1.f, 1.f}},
        {{-.05f, .05f, -.05f}, {.05f, .05f, .05f}, {0.f, 1.f}},

        // +X RIGHT (side → tile 1)
        {{.05f, -.05f, .05f}, {.05f, .05f, .05f}, {0.f, 0.f}},
        {{.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {1.f, 0.f}},
        {{.05f, .05f, -.05f}, {.05f, .05f, .05f}, {1.f, 1.f}},
        {{.05f, .05f, .05f}, {.05f, .05f, .05f}, {0.f, 1.f}},

        // +Y FRONT (tile 0)
        {{-.05f, .05f, .05f}, {.05f, .05f, .05f}, {0.f, 0.f}},
        {{.05f, .05f, .05f}, {.05f, .05f, .05f}, {1.f, 0.f}},
        {{.05f, .05f, -.05f}, {.05f, .05f, .05f}, {1.f, 1.f}},
        {{-.05f, .05f, -.05f}, {.05f, .05f, .05f}, {0.f, 1.f}},

        // -Y BACK (tile 1)
        {{-.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {0.f, 0.f}},
        {{.05f, -.05f, -.05f}, {.05f, .05f, .05f}, {1.f, 0.f}},
        {{.05f, -.05f, .05f}, {.05f, .05f, .05f}, {1.f, 1.f}},
        {{-.05f, -.05f, .05f}, {.05f, .05f, .05f}, {0.f, 1.f}},
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
