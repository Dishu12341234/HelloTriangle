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

    faces[0] = true;
    faces[1] = true;
    faces[2] = true;
    faces[3] = true;
    faces[4] = true;
    faces[5] = true;

    vertices.reserve(24);

    // +Z FRONT
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[TOP], 0);
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[TOP], 1);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[TOP], 2);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[TOP], 3);

    // -Z BACK
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BOTTOM], 0);
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BOTTOM], 1);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BOTTOM], 2);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BOTTOM], 3);

    // -X LEFT
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[RIGHT], 0);
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[RIGHT], 1);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[RIGHT], 2);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[RIGHT], 3);

    // +X RIGHT
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[LEFT], 0);
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[LEFT], 1);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[LEFT], 2);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[LEFT], 3);

    // +Y TOP
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[FRONT], 0);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[FRONT], 1);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[FRONT], 2);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[FRONT], 3);

    // -Y BOTTOM
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BACK], 0);
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BACK], 1);
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BACK], 2);
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, faceUVTextureOffsets[BACK], 3);

    indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20};

    loadGeometry(vertices, indices);
    // removeFace(TOP);
}

void StandardBoxModel::removeFace(Face face)
{
    if (!faces[face])
        return;

    faces[face] = false;

    const float s = 0.05f;
    struct FaceDef
    {
        glm::vec3 positions[4];
    };

    const FaceDef faceDefs[6] =
        {
            // FRONT (+Z) [0]
            {{{-s, -s, s}, {s, -s, s}, {s, s, s}, {-s, s, s}}},
            // BACK  (-Z) [1]
            {{{s, -s, -s}, {-s, -s, -s}, {-s, s, -s}, {s, s, -s}}},
            // LEFT  (-X) [2]
            {{{-s, -s, -s}, {-s, -s, s}, {-s, s, s}, {-s, s, -s}}},
            // RIGHT (+X) [3]
            {{{s, -s, s}, {s, -s, -s}, {s, s, -s}, {s, s, s}}},
            // TOP   (+Y) [4]
            {{{-s, s, s}, {s, s, s}, {s, s, -s}, {-s, s, -s}}},
            // BOTTOM(-Y) [5]
            {{{-s, -s, -s}, {s, -s, -s}, {s, -s, s}, {-s, -s, s}}},
        };

    vertices.clear();
    indices.clear();
    vertices.reserve(24);

    for (int f = 0; f < 6; f++)
    {
        if (!faces[f])
            continue;

        uint32_t startIndex = static_cast<uint32_t>(vertices.size());
        const FaceDef &fd = faceDefs[f];

        for (int i = 0; i < 4; i++)
        {
            vertices.emplace_back(
                fd.positions[i],
                glm::vec3{s},
                faceUVTextureOffsets[face],
                static_cast<uint32_t>(f * 4 + i) // preserve original face slot
            );
        }

        indices.push_back(startIndex + 0);
        indices.push_back(startIndex + 1);
        indices.push_back(startIndex + 2);
        indices.push_back(startIndex + 2);
        indices.push_back(startIndex + 3);
        indices.push_back(startIndex + 0);
    }

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
