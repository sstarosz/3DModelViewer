#ifndef ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP
#define ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP

#include "VulkanContext.hpp"
#include <shaderc/shaderc.hpp>

#include <string>
#include <vector>
#include <iostream>

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
        };



        //vk::Pipeline createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);


        void createShaders()
        {
            shaderc::SpvCompilationResult vertexResult = compiler.CompileGlslToSpv(vertexShader.data(), shaderc_shader_kind::shaderc_glsl_vertex_shader, "vertexShader", options);    
            shaderc::SpvCompilationResult fragmentResult = compiler.CompileGlslToSpv(fragmentShader.data(), shaderc_shader_kind::shaderc_glsl_fragment_shader, "fragmentShader", options);     


            if(vertexResult.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                throw std::runtime_error("Failed to compile vertex shader!");
                return;
            }

            if(fragmentResult.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                throw std::runtime_error("Failed to compile fragment shader!");
                return;
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


        void createDescriptorSetLayout()
        {
            std::vector<vk::DescriptorSetLayoutBinding> bindings;
            
            {// Things to do to add a new uniform buffer to the descriptor set layout
                vk::DescriptorSetLayoutBinding uboLayoutBinding {
                    0,
                    vk::DescriptorType::eUniformBuffer,
                    1,
                    vk::ShaderStageFlagBits::eVertex,
                };
            
                bindings.push_back(uboLayoutBinding);
            }


            vk::DescriptorSetLayoutCreateInfo layoutInfo{{}, bindings};


            vk::DescriptorSetLayout descriptorSetLayout = vulkanContext.m_device.createDescriptorSetLayout(layoutInfo);
        }



    private:
        vk::Pipeline createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const VkPipelineLayout& pipelineLayout, const VkRenderPass& renderPass)
        {
            /*
            //createUniformBuffers();
            //createDescriptorPool();
            //createDescriptorSetLayout(); // must stay in pipline creation
//
            //auto vertShaderCode = Shader::readFile("../Assets/Shaders/vert.spv");
            //auto fragShaderCode = Shader::readFile("../Assets/Shaders/frag.spv");
//
//
            //vk::ShaderModule vertShaderModule = Shader::createShaderModule(m_device, vertShaderCode);
            //vk::ShaderModule fragShaderModule = Shader::createShaderModule(m_device, fragShaderCode);


            vk::PipelineShaderStageCreateInfo vertShaderStageInfo { {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main" };

            vk::PipelineShaderStageCreateInfo fragShaderStageInfo { {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main" };

            std::vector<vk::PipelineShaderStageCreateInfo> shaderStages { vertShaderStageInfo, fragShaderStageInfo };
            //Customization point


            ////Create Texture Sampler if needed
            //if(false)
            //{
            //    createTextureSampler();
            //}

            

            
            auto bindingDescription = geometry::Vertex::getBindingDescription();
            auto attributeDescriptions = geometry::Vertex::getAttributeDescriptions();

            vk::PipelineVertexInputStateCreateInfo vertexInputInfo { {}, bindingDescription, attributeDescriptions };

            vk::PipelineInputAssemblyStateCreateInfo inputAssembly { vk::PipelineInputAssemblyStateCreateFlags {}, vk::PrimitiveTopology::eTriangleList, VK_FALSE };


            vk::PipelineViewportStateCreateInfo viewportState { vk::PipelineViewportStateCreateFlags {}, 1, {}, 1, {} };

            vk::PipelineRasterizationStateCreateInfo rasterizer { vk::PipelineRasterizationStateCreateFlags {},
                                                                VK_FALSE,
                                                                VK_FALSE,
                                                                vk::PolygonMode::eFill,
                                                                vk::CullModeFlagBits::eBack,
                                                                vk::FrontFace::eCounterClockwise,
                                                                VK_FALSE,
                                                                0.0f,
                                                                0.0f,
                                                                0.0f,
                                                                1.0F };

            vk::PipelineMultisampleStateCreateInfo multisampling { vk::PipelineMultisampleStateCreateFlags {}, vk::SampleCountFlagBits::e1, VK_FALSE };

            vk::PipelineDepthStencilStateCreateInfo depthStencil { {}, true, true, vk::CompareOp::eLess, false, false };

            m_dynamicStateEnables = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

            m_pipelineDynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo { {}, m_dynamicStateEnables };

            vk::PipelineColorBlendAttachmentState colorBlendAttachment { VK_FALSE,
                                                                        vk::BlendFactor::eZero,
                                                                        vk::BlendFactor::eZero,
                                                                        vk::BlendOp::eAdd,
                                                                        vk::BlendFactor::eZero,
                                                                        vk::BlendFactor::eZero,
                                                                        vk::BlendOp::eAdd,
                                                                        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                                            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };


            vk::PipelineColorBlendStateCreateInfo colorBlending {
                vk::PipelineColorBlendStateCreateFlags {},
                VK_FALSE,
                vk::LogicOp::eCopy,
                colorBlendAttachment,
                { 0.0f, 0.0f, 0.0f, 0.0f }
            };


            vk::PipelineLayoutCreateInfo pipelineLayoutInfo { vk::PipelineLayoutCreateFlags {}, m_descriptorSetLayout };

            m_pipelineLayout = m_device.createPipelineLayout(pipelineLayoutInfo);

            vk::GraphicsPipelineCreateInfo pipelineInfo { vk::PipelineCreateFlags {},
                                                        shaderStages,
                                                        &vertexInputInfo,
                                                        &inputAssembly,
                                                        {},
                                                        &viewportState,
                                                        &rasterizer,
                                                        &multisampling,
                                                        &depthStencil,
                                                        &colorBlending,
                                                        &m_pipelineDynamicStateCreateInfo,
                                                        m_pipelineLayout,
                                                        m_renderPass };


            m_pipelineCache = m_device.createPipelineCache(vk::PipelineCacheCreateInfo());
            vk::Pipeline m_graphicsPipeline = m_device.createGraphicsPipeline(m_pipelineCache, pipelineInfo).value;



            //Note VkShaderModule is passed into pipline and are not longer available trought object they are used to create
            //If ther are used later, then they must not be destroyed
            m_device.destroy(fragShaderModule);
            m_device.destroy(vertShaderModule);

            */
        }


        VulkanContext& vulkanContext;
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;


    };


    struct UniformDefinition
    {
        std::string name;
        vk::DescriptorType type;
        size_t size;
        uint32_t binding;
        uint32_t count;
        vk::ShaderStageFlagBits stage;
    };



    class PipelineBuilder
    {
        public:

        virtual void buildShader() = 0;
        virtual void buildVertexInput() = 0;
        virtual void buildDynamicState() = 0;


        vk::PipelineInputAssemblyStateCreateInfo createInputAssembly()
        {
            return vk::PipelineInputAssemblyStateCreateInfo{{}, 
                                                             vk::PrimitiveTopology::eTriangleList, 
                                                             VK_FALSE};
        }

        vk::PipelineViewportStateCreateInfo createViportState()
        {
            return vk::PipelineViewportStateCreateInfo { vk::PipelineViewportStateCreateFlags {}, 
                                                         1, 
                                                         {}, 
                                                         1, 
                                                         {} };
        }

        vk::PipelineRasterizationStateCreateInfo createRasterizationState()
        {
            return vk::PipelineRasterizationStateCreateInfo { vk::PipelineRasterizationStateCreateFlags {},
                                                             VK_FALSE,
                                                             VK_FALSE,
                                                             vk::PolygonMode::eFill,
                                                             vk::CullModeFlagBits::eBack,
                                                             vk::FrontFace::eCounterClockwise,
                                                             VK_FALSE,
                                                             0.0f,
                                                             0.0f,
                                                             0.0f,
                                                             1.0F 
                                                             };
        }

        vk::PipelineMultisampleStateCreateInfo createMultisampleState()
        {
            return vk::PipelineMultisampleStateCreateInfo { vk::PipelineMultisampleStateCreateFlags {}, 
                                                            vk::SampleCountFlagBits::e1, 
                                                            VK_FALSE 
                                                            };
        }

        vk::PipelineDepthStencilStateCreateInfo createDepthStencilState()
        {
            return vk::PipelineDepthStencilStateCreateInfo { {}, 
                                                             true, 
                                                             true, 
                                                             vk::CompareOp::eLess, 
                                                             false,      
                                                             false   
                                                             };
        }

        vk::PipelineColorBlendAttachmentState createColorBlendAttachment()
        {
            return vk::PipelineColorBlendAttachmentState { VK_FALSE,
                                                          vk::BlendFactor::eZero,
                                                          vk::BlendFactor::eZero,
                                                          vk::BlendOp::eAdd,
                                                          vk::BlendFactor::eZero,
                                                          vk::BlendFactor::eZero,
                                                          vk::BlendOp::eAdd,
                                                          vk::ColorComponentFlagBits::eR | 
                                                            vk::ColorComponentFlagBits::eG |
                                                            vk::ColorComponentFlagBits::eB | 
                                                            vk::ColorComponentFlagBits::eA 
                                                        };
        }


        vk::PipelineColorBlendStateCreateInfo createColorBlending()
        {
            return vk::PipelineColorBlendStateCreateInfo { vk::PipelineColorBlendStateCreateFlags {},
                                                           VK_FALSE,
                                                           vk::LogicOp::eCopy,
                                                           colorBlendAttachment,
                                                           { 0.0f, 0.0f, 0.0f, 0.0f }
                                                         };
        }


        vk::Pipeline createPipeline()
        {
            vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo{ vk::PipelineCreateFlags {},
                                                                       shaderStages,
                                                                       &vertexInputInfo,
                                                                       &inputAssembly,
                                                                       {},
                                                                       &viewportState,
                                                                       &rasterizer,
                                                                       &multisampling,
                                                                       &depthStencil,
                                                                       &colorBlending,
                                                                       &m_pipelineDynamicStateCreateInfo,
                                                                       pipelineLayout,
                                                                       renderPass
                                                                    };

            vk::PipelineCache pipelineCache = vulkanContext.m_device.createPipelineCache(vk::PipelineCacheCreateInfo());
            vk::Pipeline graphicsPipeline = vulkanContext.m_device.createGraphicsPipeline(pipelineCache, graphicsPipelineCreateInfo).value;
                    
        }



        virtual vk::Pipeline getPipeline() = 0;





        //Utility functions
        std::vector<vk::PipelineShaderStageCreateInfo> compileShader(const std::string& vertexShader, const std::string& fragmentShader)
        {   
            using namespace shaderc;

            //Compiler shader
            Compiler compiler;
            CompileOptions options;

            SpvCompilationResult vertexResult = compiler.CompileGlslToSpv(vertexShader, 
                                                                          shaderc_shader_kind::shaderc_glsl_vertex_shader, 
                                                                          "vertexShader", 
                                                                          options);   

            SpvCompilationResult fragmentResult = compiler.CompileGlslToSpv(fragmentShader, 
                                                                            shaderc_shader_kind::shaderc_glsl_fragment_shader, 
                                                                            "fragmentShader", 
                                                                            options);

            if(vertexResult.GetCompilationStatus() != shaderc_compilation_status_success)   
            {
                std::string errorMessage =  vertexResult.GetErrorMessage();
                
                std::cout <<"Vertex Shader Compilation Error:\n" << errorMessage << std::endl;

                //TODO handle error
                throw std::runtime_error("Failed to compile vertex shader!");
                return {};
            }

            if(fragmentResult.GetCompilationStatus() != shaderc_compilation_status_success)   
            {
                std::string errorMessage =  fragmentResult.GetErrorMessage();

                std::cout <<"Fragment Shader Compilation Error:\n" << errorMessage << std::endl;

                throw std::runtime_error("Failed to compile fragment shader!");
                return {};
            }


            //Create shader module
            std::vector<uint32_t> vertexShaderCode(vertexResult.cbegin(), vertexResult.cend());
            std::vector<uint32_t> fragmentShaderCode(fragmentResult.cbegin(), fragmentResult.cend());

            vk::ShaderModule vertexShaderModule = vulkanContext.m_device.createShaderModule({{}, vertexShaderCode});
            vk::ShaderModule fragmentShaderModule = vulkanContext.m_device.createShaderModule({{},fragmentShaderCode});

            vk::PipelineShaderStageCreateInfo vertShaderStageInfo { {}, vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main" };
		    vk::PipelineShaderStageCreateInfo fragShaderStageInfo { {}, vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main" };

            std::vector<vk::PipelineShaderStageCreateInfo> shaderStages { vertShaderStageInfo, fragShaderStageInfo };

        }

        void registerUniformBuffer(std::string name, 
                                   vk::DescriptorType type,
                                   size_t size,
                                   uint32_t binding, 
                                   uint32_t count, 
                                   vk::ShaderStageFlagBits stage)
        {
            //TODO add check for duplicate binding

            uniformDefinitions.push_back({name, type, size, binding, count, stage});
        }

        void createDescriptorSetLayout()
        {
            std::vector<vk::DescriptorSetLayoutBinding> bindings;
            
            for(const auto& uniform : uniformDefinitions)
            {
                vk::DescriptorSetLayoutBinding uboLayoutBinding {
                    uniform.binding,
                    vk::DescriptorType::eUniformBuffer,
                    uniform.count,
                    uniform.stage,
                };
            
                bindings.push_back(uboLayoutBinding);
            }

            vk::DescriptorSetLayoutCreateInfo layoutInfo{{}, bindings};
            descriptorSetLayout = vulkanContext.m_device.createDescriptorSetLayout(layoutInfo);

            vk::PipelineLayoutCreateInfo pipelineLayoutInfo { vk::PipelineLayoutCreateFlags {}, descriptorSetLayout };
            pipelineLayout = vulkanContext.m_device.createPipelineLayout(pipelineLayoutInfo);
        }

        void initializeUniformBuffers()
        {
            for(const auto& uniform : uniformDefinitions)
            {
                switch (uniform.type)
                {
                case vk::DescriptorType::eUniformBuffer:
                    createUniformBuffer(uniform);
                    break;
                case vk::DescriptorType::eCombinedImageSampler:
                    createTextureSampler(uniform);
                    break;
                }
            }
        }

        void createUniformBuffer(const UniformDefinition& uniform)
        {
            for(int i = 0; i < vulkanContext.m_swapchainImages.size(); i++)
            {
                //TODO replace with a common function for buffer creation
                vk::Buffer buffer;
                vk::DeviceMemory bufferMemory;

                vk::BufferCreateInfo bufferInfo{{}, uniform.size, vk::BufferUsageFlagBits::eUniformBuffer, vk::SharingMode::eExclusive};
                buffer = vulkanContext.m_device.createBuffer(bufferInfo);

                vk::MemoryRequirements memRequirements = vulkanContext.m_device.getBufferMemoryRequirements(buffer);

                vk::MemoryAllocateInfo allocInfo{memRequirements.size, vulkanContext.findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)};
                bufferMemory = vulkanContext.m_device.allocateMemory(allocInfo);

                vulkanContext.m_device.bindBufferMemory(buffer, bufferMemory, 0);

                uniformBuffers.push_back(buffer);
                uniformBuffersMemory.push_back(bufferMemory);
            }
        }

        void createTextureSampler(const UniformDefinition& uniform)
        {
            //TODO add option to customize sampler
            vk::SamplerCreateInfo samplerInfo{{}, 
                                              vk::Filter::eLinear, 
                                              vk::Filter::eLinear, 
                                              vk::SamplerMipmapMode::eLinear, 
                                              vk::SamplerAddressMode::eRepeat, 
                                              vk::SamplerAddressMode::eRepeat, 
                                              vk::SamplerAddressMode::eRepeat, 
                                              0.0f, 
                                              VK_TRUE, 
                                              16, 
                                              VK_FALSE, 
                                              vk::CompareOp::eAlways, 
                                              0.0f, 
                                              0.0f, 
                                              vk::BorderColor::eIntOpaqueBlack, 
                                              VK_FALSE};

            vk::Sampler textureSampler = vulkanContext.m_device.createSampler(samplerInfo);
            textureSamplers.push_back(textureSampler);
        }


        private:
        VulkanContext& vulkanContext;

        //Parent Render Pass
		const vk::RenderPass& renderPass;



        std::vector<UniformDefinition> uniformDefinitions;
        std::vector<vk::Buffer> uniformBuffers;
        std::vector<vk::DeviceMemory> uniformBuffersMemory;
        std::vector<vk::Sampler> textureSamplers;

        protected:
        //Pipeline Subproducts
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        vk::DescriptorSetLayout descriptorSetLayout;
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
        vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
        vk::PipelineViewportStateCreateInfo viewportState;
        vk::PipelineRasterizationStateCreateInfo rasterizer;
        vk::PipelineMultisampleStateCreateInfo multisampling;
        vk::PipelineDepthStencilStateCreateInfo depthStencil;
        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        vk::PipelineColorBlendStateCreateInfo colorBlending;
        vk::PipelineLayout pipelineLayout;
		vk::PipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo;

        vk::GraphicsPipelineCreateInfo pipelineInfo;

    };


    class PipelineDirector
    {
        public:
        void setBuilder(PipelineBuilder* builder)
        {
            m_builder = builder;
        }

        void constructPipeline()
        {
            /*User defined*/
            m_builder->buildShader();
            m_builder->buildVertexInput();
            m_builder->buildDynamicState();

            /*Common functionality, override for custom behavior*/
            m_builder->createInputAssembly();
            m_builder->createViportState();
            m_builder->createRasterizationState();
            m_builder->createMultisampleState();
            m_builder->createDepthStencilState();
            m_builder->createColorBlending();
            m_builder->createPipeline();
        }

        vk::Pipeline getPipeline()
        {
            return m_builder->getPipeline();
        }

        private:
        PipelineBuilder* m_builder;
    };


    
}   // namespace st::renderer

#endif //  ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP