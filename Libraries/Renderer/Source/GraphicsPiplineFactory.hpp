#ifndef ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP
#define ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP

#include "VulkanContext.hpp"
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace st::renderer
{

	struct PipelineResources
	{
		std::vector<vk::Buffer> uniformBuffers;
		std::vector<vk::DeviceMemory> uniformBuffersMemory;
		std::vector<vk::Sampler> textureSamplers;

		vk::Buffer vertexBuffer;
		vk::DeviceMemory vertexBufferMemory;

		vk::Buffer indexBuffer;
		vk::DeviceMemory indexBufferMemory;

		std::vector<vk::DescriptorSet> descriptorSets;
		vk::DescriptorPool descriptorPool;
	};

	struct PipelineCreateInformation
	{
		std::string vertexShader;
		std::string fragmentShader;
		vk::RenderPass renderPass;
	};

	class Pipeline
	{
	  public:
		// Pipeline subparts
		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
		vk::VertexInputBindingDescription vertexBindingDescription;
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
		vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
		// TODO add support for tessellation and geometry shaders
		vk::PipelineViewportStateCreateInfo viewportState;
		vk::PipelineRasterizationStateCreateInfo rasterizer;
		vk::PipelineMultisampleStateCreateInfo multisampling;
		vk::PipelineDepthStencilStateCreateInfo depthStencil;

		vk::PipelineColorBlendAttachmentState colorBlendAttachment;
		vk::PipelineColorBlendStateCreateInfo colorBlending;

		std::vector<vk::DynamicState> dynamicStates;
		vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
		vk::PipelineLayout pipelineLayout;

		// Dynamic state

		// Descriptor and resources
		vk::DescriptorSetLayout descriptorSetLayout;
		PipelineResources resources;

		vk::Pipeline pipeline;
	};

	class PipelineBuilder
	{
	  public:
		PipelineBuilder(const VulkanContext& vulkanContext, const vk::RenderPass& renderPass) :
			vulkanContext(vulkanContext),
			renderPass(renderPass)
		{
		}

		virtual ~PipelineBuilder() = default;

		virtual void buildShader() = 0;
		virtual void buildDynamicState() = 0;

		void createInputAssembly()
		{
			pipeline.inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{vk::PipelineInputAssemblyStateCreateFlags{}, vk::PrimitiveTopology::eTriangleList, VK_FALSE};
		}

		void createViewportState()
		{
			pipeline.viewportState = vk::PipelineViewportStateCreateInfo{vk::PipelineViewportStateCreateFlags{}, 1, {}, 1, {}};
		}

		void createRasterizationState()
		{
			pipeline.rasterizer = vk::PipelineRasterizationStateCreateInfo{vk::PipelineRasterizationStateCreateFlags{},
																		   VK_FALSE,
																		   VK_FALSE,
																		   vk::PolygonMode::eFill,
																		   vk::CullModeFlagBits::eFront,
																		   vk::FrontFace::eCounterClockwise,
																		   VK_FALSE,
																		   0.0f,
																		   0.0f,
																		   0.0f,
																		   1.0F};
		}

		void createMultisampleState()
		{
			pipeline.multisampling = vk::PipelineMultisampleStateCreateInfo{vk::PipelineMultisampleStateCreateFlags{}, vk::SampleCountFlagBits::e1, VK_FALSE};
		}

		void createDepthStencilState()
		{
			pipeline.depthStencil = vk::PipelineDepthStencilStateCreateInfo{{}, true, true, vk::CompareOp::eLess, false, false};
		}

		void createColorBlending()
		{
			pipeline.colorBlendAttachment = vk::PipelineColorBlendAttachmentState{VK_FALSE,
																				  vk::BlendFactor::eZero,
																				  vk::BlendFactor::eZero,
																				  vk::BlendOp::eAdd,
																				  vk::BlendFactor::eZero,
																				  vk::BlendFactor::eZero,
																				  vk::BlendOp::eAdd,
																				  vk::ColorComponentFlagBits::eR |
																					  vk::ColorComponentFlagBits::eG |
																					  vk::ColorComponentFlagBits::eB |
																					  vk::ColorComponentFlagBits::eA};

			pipeline.colorBlending = vk::PipelineColorBlendStateCreateInfo{vk::PipelineColorBlendStateCreateFlags{},
																		   VK_FALSE,
																		   vk::LogicOp::eCopy,
																		   pipeline.colorBlendAttachment,
																		   {{1.0f, 1.0f, 1.0f, 1.0f}}};
		}

		void createPipeline()
		{
			vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo{vk::PipelineCreateFlags{},
																	  pipeline.shaderStages,
																	  &pipeline.vertexInputInfo,
																	  &pipeline.inputAssembly,
																	  {},
																	  &pipeline.viewportState,
																	  &pipeline.rasterizer,
																	  &pipeline.multisampling,
																	  &pipeline.depthStencil,
																	  &pipeline.colorBlending,
																	  &pipeline.dynamicStateInfo,
																	  pipeline.pipelineLayout,
																	  renderPass};

			vk::PipelineCache pipelineCache = vulkanContext.m_device.createPipelineCache(vk::PipelineCacheCreateInfo());
			pipeline.pipeline = vulkanContext.m_device.createGraphicsPipeline(pipelineCache, graphicsPipelineCreateInfo).value;
		}

		// Utility functions
		std::vector<vk::PipelineShaderStageCreateInfo> compileShader(const std::string& vertexShader, const std::string& fragmentShader)
		{

			// Compile shaders
			std::vector<uint32_t> vertexShaderSpvCode = compileShader(vertexShader, shaderc_shader_kind::shaderc_glsl_vertex_shader);
			std::vector<uint32_t> fragmentShaderSpvCode = compileShader(fragmentShader, shaderc_shader_kind::shaderc_glsl_fragment_shader);

			vk::ShaderModule vertexShaderModule = vulkanContext.m_device.createShaderModule({{}, vertexShaderSpvCode});
			vk::ShaderModule fragmentShaderModule = vulkanContext.m_device.createShaderModule({{}, fragmentShaderSpvCode});

			vk::PipelineShaderStageCreateInfo vertShaderStageInfo{{}, vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main"};
			vk::PipelineShaderStageCreateInfo fragShaderStageInfo{{}, vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main"};

			std::vector<vk::PipelineShaderStageCreateInfo> shaderStages{vertShaderStageInfo, fragShaderStageInfo};

			// Define the descriptor set layout
			std::vector<vk::DescriptorSetLayoutBinding> bindings;
			addShaderToDescriptorSetLayout(bindings, vk::ShaderStageFlagBits::eVertex, vertexShaderSpvCode);
			addShaderToDescriptorSetLayout(bindings, vk::ShaderStageFlagBits::eFragment, fragmentShaderSpvCode);

			vk::DescriptorSetLayoutCreateInfo layoutInfo{vk::DescriptorSetLayoutCreateFlags{}, bindings};
			pipeline.descriptorSetLayout = vulkanContext.m_device.createDescriptorSetLayout(layoutInfo);

			vk::PipelineLayoutCreateInfo pipelineLayoutInfo{vk::PipelineLayoutCreateFlags{}, pipeline.descriptorSetLayout};
			pipeline.pipelineLayout = vulkanContext.m_device.createPipelineLayout(pipelineLayoutInfo);

			// Create Resources (Uniform Buffers / Texture Samplers)
			initializeResourcesForShader(vertexShaderSpvCode);

			// Create Vertex Input
			buildVertexInput(vertexShaderSpvCode);

			return shaderStages;
		}

		std::vector<uint32_t> compileShader(const std::string& shaderCode, shaderc_shader_kind shaderType)
		{
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;

			shaderc::SpvCompilationResult compiledSpv = compiler.CompileGlslToSpv(shaderCode, shaderType, "shader", options);

			if (compiledSpv.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				std::string errorMessage = compiledSpv.GetErrorMessage();

				std::cout << "Shader Compilation Error:\n"
						  << errorMessage << std::endl;

				// TODO handle error
				throw std::runtime_error("Failed to compile shader!");
			}

			return {compiledSpv.cbegin(), compiledSpv.cend()};
		}

		void addShaderToDescriptorSetLayout(std::vector<vk::DescriptorSetLayoutBinding>& bindings, vk::ShaderStageFlagBits stage, std::vector<uint32_t>& shaderSpvCode)
		{
			// TODO add check for other shader languages
			spirv_cross::CompilerGLSL crossCompiler{shaderSpvCode};
			spirv_cross::ShaderResources shaderResources = crossCompiler.get_shader_resources();

			for (const auto& resource : shaderResources.uniform_buffers)
			{
				const spirv_cross::SPIRType& type = crossCompiler.get_type(resource.base_type_id);

				const uint32_t set = crossCompiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
				const uint32_t binding = crossCompiler.get_decoration(resource.id, spv::DecorationBinding);

				uint32_t count = 1;
				if (!type.array.empty())
				{
					count = type.array[0];
				}

				vk::DescriptorSetLayoutBinding uboLayoutBinding{binding, vk::DescriptorType::eUniformBuffer, count, stage};

				bindings.push_back(uboLayoutBinding);
			}

			for (const auto& resource : shaderResources.sampled_images)
			{
				const spirv_cross::SPIRType& type = crossCompiler.get_type(resource.base_type_id);
				const uint32_t set = crossCompiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
				const uint32_t binding = crossCompiler.get_decoration(resource.id, spv::DecorationBinding);

				uint32_t count = 1;
				if (!type.array.empty())
				{
					count = type.array[0];
				}

				vk::DescriptorSetLayoutBinding samplerLayoutBinding{binding, vk::DescriptorType::eCombinedImageSampler, count, stage};

				bindings.push_back(samplerLayoutBinding);
			}
		}

		void initializeResourcesForShader(std::vector<uint32_t>& shaderSpvCode)
		{
			spirv_cross::CompilerGLSL crossCompiler{shaderSpvCode};
			spirv_cross::ShaderResources shaderResources = crossCompiler.get_shader_resources();

			for (const auto& resource : shaderResources.uniform_buffers)
			{
				const spirv_cross::SPIRType& type = crossCompiler.get_type(resource.base_type_id);
				size_t uniformSize = crossCompiler.get_declared_struct_size(type);
				createUniformBuffer(uniformSize);
			}

			for (const auto& resource : shaderResources.sampled_images)
			{
				createTextureSampler();
			}
		}

		void createUniformBuffer(const size_t& uniformSize)
		{
			for (int i = 0; i < vulkanContext.MAX_FRAMES_IN_FLIGHT; i++)
			{
				// TODO replace with a common function for buffer creation
				vk::Buffer buffer;
				vk::DeviceMemory bufferMemory;

				vk::BufferCreateInfo bufferInfo{vk::BufferCreateFlags{}, uniformSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::SharingMode::eExclusive};

				buffer = vulkanContext.m_device.createBuffer(bufferInfo);

				vk::MemoryRequirements memRequirements = vulkanContext.m_device.getBufferMemoryRequirements(buffer);

				vk::MemoryAllocateInfo allocInfo{memRequirements.size,
												 vulkanContext.findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)};

				bufferMemory = vulkanContext.m_device.allocateMemory(allocInfo);

				vulkanContext.m_device.bindBufferMemory(buffer, bufferMemory, 0);

				pipeline.resources.uniformBuffers.push_back(buffer);
				pipeline.resources.uniformBuffersMemory.push_back(bufferMemory);
			}
		}

		void createTextureSampler()
		{
			// TODO add option to customize sampler
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
			pipeline.resources.textureSamplers.push_back(textureSampler);
		}

		void buildVertexInput(std::vector<uint32_t>& shaderSpvCode)
		{
			spirv_cross::CompilerGLSL crossCompiler(shaderSpvCode);
			spirv_cross::ShaderResources shaderResources = crossCompiler.get_shader_resources();

			size_t totalSize = 0;
			for (const auto& resource : shaderResources.stage_inputs)
			{
				const spirv_cross::SPIRType& type = crossCompiler.get_type(resource.base_type_id);

				uint32_t location = crossCompiler.get_decoration(resource.id, spv::DecorationLocation);
				uint32_t binding = crossCompiler.get_decoration(resource.id, spv::DecorationBinding);
				vk::Format format = spirvTypeToVkFormat(type);
				uint32_t offset = crossCompiler.get_decoration(resource.id, spv::DecorationOffset);
				totalSize += getTypeSize(type);

				vk::VertexInputAttributeDescription attributeDescription{location, binding, format, offset};
				pipeline.attributeDescriptions.push_back(attributeDescription);
			}

			pipeline.vertexBindingDescription = vk::VertexInputBindingDescription{0, static_cast<uint32_t>(totalSize), vk::VertexInputRate::eVertex};

			pipeline.vertexInputInfo = vk::PipelineVertexInputStateCreateInfo{vk::PipelineVertexInputStateCreateFlags{}, pipeline.vertexBindingDescription, pipeline.attributeDescriptions};
		}

		vk::Format spirvTypeToVkFormat(const spirv_cross::SPIRType& type)
		{
			if (type.basetype == spirv_cross::SPIRType::Float)
			{
				if (type.vecsize == 1 && type.columns == 1)
				{
					return vk::Format::eR32Sfloat;
				}
				if (type.vecsize == 2 && type.columns == 1)
				{
					return vk::Format::eR32G32Sfloat;
				}
				if (type.vecsize == 3 && type.columns == 1)
				{
					return vk::Format::eR32G32B32Sfloat;
				}
				if (type.vecsize == 4 && type.columns == 1)
				{
					return vk::Format::eR32G32B32A32Sfloat;
				}
			}
			// Add more cases for other types...

			throw std::runtime_error("Unsupported SPIR-V type");
		}

		uint32_t getTypeSize(const spirv_cross::SPIRType& type)
		{
			uint32_t elementSize = 0;
			switch (type.basetype)
			{
			case spirv_cross::SPIRType::Float: elementSize = 4; break;	// float is 4 bytes
			case spirv_cross::SPIRType::Int: elementSize = 4; break;	// int is 4 bytes
			case spirv_cross::SPIRType::UInt: elementSize = 4; break;	// uint is 4 bytes
			case spirv_cross::SPIRType::Double: elementSize = 8; break; // double is 8 bytes
			case spirv_cross::SPIRType::Boolean:
				elementSize = 1;
				break; // bool is 1 byte
			// TODO Add more cases for other base types...
			default: throw std::runtime_error("Unsupported base type");
			}

			return elementSize * type.vecsize; // vec3 has 3 elements
		}

		void allocateResources()
		{
			vk::DescriptorPoolSize poolSize{vk::DescriptorType::eUniformBuffer, vulkanContext.MAX_FRAMES_IN_FLIGHT * 2};

			vk::DescriptorPoolCreateInfo poolInfo{vk::DescriptorPoolCreateFlags{}, vulkanContext.MAX_FRAMES_IN_FLIGHT * 2, poolSize};
			pipeline.resources.descriptorPool = vulkanContext.m_device.createDescriptorPool(poolInfo);

			std::vector<vk::DescriptorSetLayout> layouts{vulkanContext.MAX_FRAMES_IN_FLIGHT, pipeline.descriptorSetLayout};
			vk::DescriptorSetAllocateInfo allocInfo{pipeline.resources.descriptorPool, layouts};
			pipeline.resources.descriptorSets = vulkanContext.m_device.allocateDescriptorSets(allocInfo);

			for (int i = 0; i < vulkanContext.MAX_FRAMES_IN_FLIGHT; i++)
			{
				vk::DescriptorBufferInfo bufferInfo{pipeline.resources.uniformBuffers[i], 0, VK_WHOLE_SIZE};

				vk::WriteDescriptorSet descriptorWrite{pipeline.resources.descriptorSets[i], 0, 0, vk::DescriptorType::eUniformBuffer, {}, bufferInfo, {}};
				vulkanContext.m_device.updateDescriptorSets(descriptorWrite, {});
			}
		}

		//--------------------------------------------------------------------------------
		//---------------------------------Debug------------------------------------------
		//--------------------------------------------------------------------------------
		void printDynamicStateCreateInfo(const vk::PipelineDynamicStateCreateInfo& dynamicStateInfo)
		{
			std::cout << "Dynamic State Create Info:\n";
			// std::cout << "  Flags: " << to_string(dynamicStateInfo.flags) <<
			// "\n";
			std::cout << "  Dynamic State Count: " << dynamicStateInfo.dynamicStateCount << "\n";
			std::cout << "  Dynamic States:\n";
			for (uint32_t i = 0; i < dynamicStateInfo.dynamicStateCount; ++i)
			{
				std::cout << "    " << vk::to_string(dynamicStateInfo.pDynamicStates[i]) << "\n";
			}
		}

		Pipeline getPipeline()
		{
			return pipeline;
		}

	  protected:
		const vk::RenderPass& renderPass;
		const VulkanContext& vulkanContext;

		// Pipeline to be built
		Pipeline pipeline;
	};

	class PipelineDirector
	{
	  public:
		void setBuilder(PipelineBuilder* builder)
		{
			this->builder = builder;
		}

		void constructPipeline()
		{
			/*User defined*/
			builder->buildShader();
			builder->buildDynamicState();

			/*Common functionality, override for custom behavior*/
			builder->createInputAssembly();
			builder->createViewportState();
			builder->createRasterizationState();
			builder->createMultisampleState();
			builder->createDepthStencilState();
			builder->createColorBlending();
			builder->createPipeline();

			/*Allocate resources*/
			builder->allocateResources();
		}

		Pipeline getPipeline()
		{
			return builder->getPipeline();
		}

	  private:
		PipelineBuilder* builder;
	};

} // namespace st::renderer

#endif //  ST_RENDERER_GRAPHICS_PIPELINE_FACTORY_HPP