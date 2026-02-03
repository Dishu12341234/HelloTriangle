#include "models/StandarBoxModel.h"


inline glm::vec2 tileUV(uint32_t tileIndex, bool max)
{
    uint32_t x = tileIndex % TILES_PER_ROW;
    uint32_t y = tileIndex / TILES_PER_ROW;

    float u = (x + (max ? 1 : 0)) * U;
    float v = (y + (max ? 1 : 0)) * U;

    return {u, v};
}

StandardBoxModel::StandardBoxModel(std::array<float, 6> faceUVTextureOffsets, VulkanContext vkContext) : GameObject{vkContext}
{
    this->vkContext = vkContext;

    auto uv0 = [&](int face){ return tileUV(faceUVTextureOffsets[face], false); };
    auto uv1 = [&](int face){ return tileUV(faceUVTextureOffsets[face], true ); };

    std::vector<Vertex> vertices = {

        // +Z FRONT
        {{-.05, -.05,  .05}, {.05,.05,.05}, uv0(FRONT)},
        {{ .05, -.05,  .05}, {.05,.05,.05}, {uv1(FRONT).x, uv0(FRONT).y}},
        {{ .05,  .05,  .05}, {.05,.05,.05}, uv1(FRONT)},
        {{-.05,  .05,  .05}, {.05,.05,.05}, {uv0(FRONT).x, uv1(FRONT).y}},

        // -Z BACK
        {{ .05, -.05, -.05}, {.05,.05,.05}, uv0(BACK)},
        {{-.05, -.05, -.05}, {.05,.05,.05}, {uv1(BACK).x, uv0(BACK).y}},
        {{-.05,  .05, -.05}, {.05,.05,.05}, uv1(BACK)},
        {{ .05,  .05, -.05}, {.05,.05,.05}, {uv0(BACK).x, uv1(BACK).y}},

        // -X LEFT
        {{-.05, -.05, -.05}, {.05,.05,.05}, uv0(LEFT)},
        {{-.05, -.05,  .05}, {.05,.05,.05}, {uv1(LEFT).x, uv0(LEFT).y}},
        {{-.05,  .05,  .05}, {.05,.05,.05}, uv1(LEFT)},
        {{-.05,  .05, -.05}, {.05,.05,.05}, {uv0(LEFT).x, uv1(LEFT).y}},

        // +X RIGHT
        {{ .05, -.05,  .05}, {.05,.05,.05}, uv0(RIGHT)},
        {{ .05, -.05, -.05}, {.05,.05,.05}, {uv1(RIGHT).x, uv0(RIGHT).y}},
        {{ .05,  .05, -.05}, {.05,.05,.05}, uv1(RIGHT)},
        {{ .05,  .05,  .05}, {.05,.05,.05}, {uv0(RIGHT).x, uv1(RIGHT).y}},

        // +Y TOP
        {{-.05, .05,  .05}, {.05,.05,.05}, uv0(TOP)},
        {{ .05, .05,  .05}, {.05,.05,.05}, {uv1(TOP).x, uv0(TOP).y}},
        {{ .05, .05, -.05}, {.05,.05,.05}, uv1(TOP)},
        {{-.05, .05, -.05}, {.05,.05,.05}, {uv0(TOP).x, uv1(TOP).y}},

        // -Y BOTTOM
        {{-.05, -.05, -.05}, {.05,.05,.05}, uv0(BOTTOM)},
        {{ .05, -.05, -.05}, {.05,.05,.05}, {uv1(BOTTOM).x, uv0(BOTTOM).y}},
        {{ .05, -.05,  .05}, {.05,.05,.05}, uv1(BOTTOM)},
        {{-.05, -.05,  .05}, {.05,.05,.05}, {uv0(BOTTOM).x, uv1(BOTTOM).y}},
    };

    std::vector<uint32_t> indices = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };

    loadGeometry(vertices, indices);
}

void StandardBoxModel::cleanUpResources()
{
    GameObject::cleanUpResources();
}

StandardBoxModel::~StandardBoxModel()
{
}
