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
    std::vector<Vertex> vertices;
    vertices.reserve(24);

    // +Z FRONT
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 0.f}, 0);
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 0.f}, 1);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 1.f}, 2);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 1.f}, 3);

    // -Z BACK
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 0.f}, 4);
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 0.f}, 5);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 1.f}, 6);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 1.f}, 7);

    // -X LEFT
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 0.f}, 8);
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 0.f}, 9);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 1.f}, 10);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 1.f}, 11);

    // +X RIGHT
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 0.f}, 12);
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 0.f}, 13);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 1.f}, 14);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 1.f}, 15);

    // +Y TOP
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 0.f}, 16);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 0.f}, 17);
    vertices.emplace_back(glm::vec3{0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 1.f}, 18);
    vertices.emplace_back(glm::vec3{-0.05f, 0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 1.f}, 19);

    // -Y BOTTOM
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 0.f}, 20);
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, -0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 0.f}, 21);
    vertices.emplace_back(glm::vec3{0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{1.f, 1.f}, 22);
    vertices.emplace_back(glm::vec3{-0.05f, -0.05f, 0.05f}, glm::vec3{0.05f}, glm::vec2{0.f, 1.f}, 23);

    std::vector<uint32_t> indices = {
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
    size_t vertexStart = face * 4;
    size_t indexStart  = face * 6;
    
    // Remove indices
    mesh->indices.erase(
        mesh->indices.begin() + indexStart,
        mesh->indices.begin() + indexStart + 6
    );

    // Remove vertices
    mesh->vertices.erase(
        mesh->vertices.begin() + vertexStart,
        mesh->vertices.begin() + vertexStart + 4
    );

    // Fix shifted indices
    for (auto& idx : mesh->indices)
    {
        if (idx > vertexStart)
            idx -= 4;
    }

    // Recreate GPU buffers
    loadGeometry(mesh->vertices, mesh->indices);
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
