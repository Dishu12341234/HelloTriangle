#ifndef GAME_OBJECT
#define GAME_OBJECT
#include "GameMeshObject.h"
#include "MeshUploader.h"

struct d_Pos3D
{
    int x, y, z;
};

struct d_Rect3D // dimessions_rect
{
    int x, y, z;
    int w, h, b;
};

struct Transform
{
    glm::vec3 position;
};


enum Face
{
    TOP = 0,
    BOTTOM = 1,
    LEFT = 2,
    RIGHT = 3,
    FRONT = 4,
    BACK = 5,
};


class GameObject
{
private:
    static std::atomic<uint32_t> globalGOIDCounter;
    uint32_t GOID = UINT32_MAX;
    d_Pos3D position;
    VulkanContext vkContext;
    static MeshUploader meshUploader;
    std::vector<PendingUpload> uploadGarbage;
    friend class StandardBoxModel;
    friend class GameObjectPool;

public:
    uint32_t faceUVTextureOffsets[6];
    int tileIndex{0};

    GameMeshObject *mesh = nullptr;
    Transform transform;

    GameObject(VulkanContext vkContext);

    void loadMesh(GameMeshObject *mesh);
    void loadGeometry(std::string);
    void loadGeometry(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
    
    void initUpload();
    void drawIndexed(VkCommandBuffer &commandBuffer, std::vector<VkDescriptorSet> &descriptorSets, u_GraphicsPipeline &graphicsPipeline, VkExtent2D &swapChainExtent, uint64_t &instanceCount, uint32_t &currentFrame);

    uint32_t getID();
    // void removeFace(Face face);

    virtual void cleanUpResources();
    virtual ~GameObject() = default;
};

#endif