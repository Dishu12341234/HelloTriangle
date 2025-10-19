#ifndef H_GPIPE
#define H_GPIPE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <fstream>

struct u_GraphicsPipelineCreateInfo
{
    VkDevice device;
    int height, width;
    VkExtent2D swapChainExtent;;
};

class u_GraphicsPipeline
{
private:
    VkDevice device;
    int height, width;
    VkExtent2D swapChainExtent;
    VkPipelineLayout pipelineLayout;


public:
    u_GraphicsPipeline();
    ~u_GraphicsPipeline();
    void destroyPipelineLayout();

    void u_PassGraphicsPipelineCreateInfo(u_GraphicsPipelineCreateInfo pCreateInfo);

    static std::vector<char> readFile(const std::string &filename);

    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<char> &code);
};
#endif