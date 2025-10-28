#ifndef H_GPIPE
#define H_GPIPE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

#include <vector>
#include <fstream>

//UBO
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    //Telling vulkan how to setup binding
    //Struct 1/2
    static VkVertexInputBindingDescription getBindingDescription();
    //Struct 2/2
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

struct u_GraphicsPipelineCreateInfo
{
    VkDevice device;
    int height, width;
    VkExtent2D swapChainExtent;
    VkRenderPass renderPass;
    VkDescriptorSetLayout* descriptorSetLayout;
};

class u_GraphicsPipeline
{
private:
    VkDevice device;
    int height, width;
    VkExtent2D swapChainExtent;
    VkRenderPass renderPass;
    VkDescriptorSetLayout* descriptorSetLayout;
    
    public:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;//shared
    
    u_GraphicsPipeline();
    ~u_GraphicsPipeline();
    void destroyPipelineLayout();

    void u_PassGraphicsPipelineCreateInfo(u_GraphicsPipelineCreateInfo pCreateInfo);

    static std::vector<char> readFile(const std::string &filename);

    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<char> &code);
};
#endif