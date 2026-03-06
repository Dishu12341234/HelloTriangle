#include "models/StandarBoxModel.h"

StandardBoxModel::StandardBoxModel(std::vector<uint32_t> faceUVTextureOffsetsa, VulkanContext vkContext) : GameObject{vkContext}
{
    objectType = ObjectType::StandardBoxModel;

    this->vkContext = vkContext;
    faceUVTextureOffsets[0] = faceUVTextureOffsetsa[0];
    faceUVTextureOffsets[1] = faceUVTextureOffsetsa[1];
    faceUVTextureOffsets[2] = faceUVTextureOffsetsa[2];
    faceUVTextureOffsets[3] = faceUVTextureOffsetsa[3];
    faceUVTextureOffsets[4] = faceUVTextureOffsetsa[4];
    faceUVTextureOffsets[5] = faceUVTextureOffsetsa[5];

    faces[0] = false;
    faces[1] = false;
    faces[2] = false;
    faces[3] = false;
    faces[4] = false;
    faces[5] = false;

    vertices.reserve(24);
    indices.reserve(36);

    // // +Z FRONT
    // vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[TOP], 0);
    // vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[TOP], 1);
    // vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[TOP], 2);
    // vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[TOP], 3);

    // // -Z BACK
    // vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BOTTOM], 0);
    // vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BOTTOM], 1);
    // vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BOTTOM], 2);
    // vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BOTTOM], 3);

    // // -X LEFT
    // vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[RIGHT], 0);
    // vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[RIGHT], 1);
    // vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[RIGHT], 2);
    // vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[RIGHT], 3);

    // // +X RIGHT
    // vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[LEFT], 0);
    // vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[LEFT], 1);
    // vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[LEFT], 2);
    // vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[LEFT], 3);

    // // +Y TOP
    // vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[FRONT], 0);
    // vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[FRONT], 1);
    // vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[FRONT], 2);
    // vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[FRONT], 3);

    // // -Y BOTTOM
    // vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BACK], 0);
    // vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BACK], 1);
    // vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BACK], 2);
    // vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BACK], 3);

    // indices = {
    //     0, 1, 2, 2, 3, 0,
    //     4, 5, 6, 6, 7, 4,
    //     8, 9, 10, 10, 11, 8,
    //     12, 13, 14, 14, 15, 12,
    //     16, 17, 18, 18, 19, 16,
    //     20, 21, 22, 22, 23, 20};

    // loadGeometry(vertices, indices);
    // removeFace(TOP);
}

static constexpr float s = 0.05f;

struct FaceDef
{
    glm::vec3 positions[4];
};

static const FaceDef faceDefs[6] = {
    {{{-s, -s, s}, {s, -s, s}, {s, s, s}, {-s, s, s}}},     // FRONT
    {{{s, -s, -s}, {-s, -s, -s}, {-s, s, -s}, {s, s, -s}}}, // BACK
    {{{-s, -s, -s}, {-s, -s, s}, {-s, s, s}, {-s, s, -s}}}, // LEFT
    {{{s, -s, s}, {s, -s, -s}, {s, s, -s}, {s, s, s}}},     // RIGHT
    {{{-s, s, s}, {s, s, s}, {s, s, -s}, {-s, s, -s}}},     // TOP
    {{{-s, -s, -s}, {s, -s, -s}, {s, -s, s}, {-s, -s, s}}}, // BOTTOM
};

void StandardBoxModel::removeFace(Face face)
{
    if (!faces[face])
        return;

    faces[face] = false;

    vertices.clear();
    indices.clear();
    vertices.reserve(24);
    indices.reserve(36);

    for (int f = 0; f < 6; ++f)
    {
        if (!faces[f])
            continue;

        const FaceDef &fd = faceDefs[f];
        const auto uv = faceUVTextureOffsets[f];

        uint32_t startIndex = vertices.size();

        for (int i = 0; i < 4; ++i)
        {
            vertices.emplace_back(
                fd.positions[i],
                glm::vec3{s},
                uv,
                f * 4 + i);
        }

        indices.insert(indices.end(), {startIndex + 0, startIndex + 1, startIndex + 2,
                                       startIndex + 2, startIndex + 3, startIndex + 0});
    }

    loadGeometry(vertices, indices);
}

void StandardBoxModel::addFace(Face face, int atlasIndex)
{
    if (faces[face])
        return;

    faces[face] = true;

    const FaceDef &fd = faceDefs[face];

    uint32_t startIndex = vertices.size();

    for (int i = 0; i < 4; i++)
    {
        vertices.emplace_back(
            fd.positions[i],
            glm::vec3{s},
            faceUVTextureOffsets[atlasIndex],
            face * 4 + i);
    }

    indices.insert(indices.end(), {startIndex + 0,
                                   startIndex + 1,
                                   startIndex + 2,
                                   startIndex + 2,
                                   startIndex + 3,
                                   startIndex + 0});
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
