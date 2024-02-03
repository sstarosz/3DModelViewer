#ifndef ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP
#define ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP

#include "VulkanContext.hpp"
#include <shaderc/shaderc.hpp>

#include <string>

namespace st::renderer
{

    std::string_view vertexShader= R"(
        #version 450

        layout(location = 0) in vec3 inPosition;
        layout(location = 1) in vec3 inColor;

        layout(location = 0) out vec3 outColor;

        layout(set = 0, binding = 0) uniform MVP {
            mat4 model;
            mat4 view;
            mat4 proj;
        } mvp;


        void main() 
        {
            gl_Position = mvp.proj * mvp.view * mvp.model * vec4(inPosition, 1.0);
            outColor = inColor;
        } 
    )";

    std::string_view fragmentShader = R"(
        #version 450

        layout(location = 0) in vec3 inColor;

        layout(location = 0) out vec4 outColor;

        void main() 
        {
            outColor = vec4(inColor, 1.0);
        } 
    )";


    //Compiler shader
    //Register Uniform / Uniform Buffer Object

    //Register Descriptor Set (Define the layout of input data to the shader)
    //Register Descriptor Set Layout
    //Register Descriptor Pool


    class GraphicsPipelineFactory
    {
    public:
        GraphicsPipelineFactory(VulkanContext& vulkanContext) : vulkanContext(vulkanContext)
        {

        }

        struct GraphicsPipeline
        {
            vk::Pipeline pipeline;
            vk::PipelineLayout pipelineLayout;
            vk::PipelineCache pipelineCache;

            vk::DescriptorPool descriptorPool;
            vk::DescriptorSetLayout descriptorSetLayout;

            //std::vector<vk::Buffer> uniformBuffers;
            //std::vector<vk::DeviceMemory> uniformBuffersMemory;
            //
            //vk::Sampler textureSampler;
        }



        //vk::Pipeline createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);


        void createShaders()
        {
            shaderc::SpvCompilationResult vertexResult = compiler.CompileGlslToSpv(vertexShader.data(), shaderc_shader_kind::shaderc_glsl_vertex_shader, "vertexShader", options);    
            shaderc::SpvCompilationResult fragmentResult = compiler.CompileGlslToSpv(fragmentShader.data(), shaderc_shader_kind::shaderc_glsl_fragment_shader, "fragmentShader", options);     


            if(vertexResult.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                throw std::runtime_error("Failed to compile vertex shader!");
                return
            }

            if(fragmentResult.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                throw std::runtime_error("Failed to compile fragment shader!");
                return
            }

            std::vector<uint32_t> vertexShaderCode(vertexResult.cbegin(), vertexResult.cend());
            std::vector<uint32_t> fragmentShaderCode(fragmentResult.cbegin(), fragmentResult.cend());

            vk::ShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
            vk::ShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);

        }

        vk::ShaderModule createShaderModule(const std::vector<uint32_t>& code)
        {
            vk::ShaderModuleCreateInfo createInfo{{}, code};

            vk::ShaderModule shaderModule = vulkanContext.m_device.createShaderModule(createInfo);
            return shaderModule;
        }




    private:
        vk::Pipeline createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const VkPipelineLayout& pipelineLayout, const VkRenderPass& renderPass);


        VulkanContext& vulkanContext;
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;


    };



    
}   // namespace st::renderer

#endif //  ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP