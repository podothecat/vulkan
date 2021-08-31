#include "lve_pipeline.hpp"

// std
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve
{
    LvePipeline::LvePipeline(LveDevice &device,
                             const std::string &vertFilePath,
                             const std::string &fragFilePath,
                             const PipelineConfigInfo &configInfo) : device(device)
    {
        createGraphicsPipeLine(vertFilePath, fragFilePath, configInfo);
    }

    std::vector<char> LvePipeline::readFile(const std::string &filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("fail to open a file: " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    void LvePipeline::createGraphicsPipeLine(
        const std::string &vertFilePath,
        const std::string &fragFilePath,
        const PipelineConfigInfo &configInfo)
    {
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        std::cout << "Vertex Shader Code Size : " << vertCode.size() << "\n";
        std::cout << "Fragment Shader Code Size : " << fragCode.size() << "\n";
    }

    void LvePipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("fail to create shader module");
        }
    }

    PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo info{};

        info.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        info.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        info.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        info.viewport.x = 0.0f;
        info.viewport.y = 0.0f;
        info.viewport.width = static_cast<float>(width);
        info.viewport.height = static_cast<float>(height);
        info.viewport.minDepth = 0.0f;
        info.viewport.maxDepth = 1.0f;

        info.scissor.offset = {0, 0};
        info.scissor.extent = {width, height};

        info.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        info.viewportInfo.viewportCount = 1;
        info.viewportInfo.pViewports = &info.viewport;
        info.viewportInfo.scissorCount = 1;
        info.viewportInfo.pScissors = &info.scissor;

        info.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        info.rasterizationInfo.depthClampEnable = VK_FALSE;
        info.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        info.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        info.rasterizationInfo.lineWidth = 1.0f;
        info.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        info.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        info.rasterizationInfo.depthBiasEnable = VK_FALSE;
        info.rasterizationInfo.depthBiasConstantFactor = 0.0f;
        info.rasterizationInfo.depthBiasClamp = 0.0f;
        info.rasterizationInfo.depthBiasSlopeFactor = 0.0f;
        

        return info;
    }
}