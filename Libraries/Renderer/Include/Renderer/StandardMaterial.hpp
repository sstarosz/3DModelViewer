#ifndef ST_RENDERER_STANDARD_MATERIAL_HPP
#define ST_RENDERER_STANDARD_MATERIAL_HPP

#include "GraphicsPiplineFactory.hpp"
#include "Material.hpp"
#include "Renderable.hpp"
#include "Core/Nodes/Node.hpp"
#include "Core/Mesh.hpp"

#include <string_view>
#include <string>

namespace st::renderer
{

	constexpr std::string_view standardMaterialVertexShader = R"(
		#version 450

		layout(location = 0) in vec3 inPosition;
		//layout(location = 1) in vec3 inColor;

		layout(location = 0) out vec3 outFragColor;

		layout(set = 0, binding = 0) uniform MVP
		{
			mat4 model;
			mat4 view;
			mat4 proj;
		} mvp;

		void main()
		{
			gl_Position = vec4(inPosition, 1.0);
			outFragColor = vec3(1.0, 0.0, 0.0);
		}
    )";

	constexpr std::string_view standardMaterialFragmentShader = R"(
        #version 450

		layout(location = 0) in vec3 inFragColor;

		layout(location = 0) out vec4 outColor;

		void main()
		{
			outColor = vec4(inFragColor, 1.0);
		}
    )";

	class StandardMaterial : public Material
	{
	  public:
		StandardMaterial() = default;

	  private:
		class StandardMaterialBuilder : public PipelineBuilder
		{
		  public:
			StandardMaterialBuilder(const VulkanContext& context, const vk::RenderPass& renderPass) :
				PipelineBuilder(context, renderPass)
			{
			}

			void buildShader() override
			{
				// Compile shaders
				pipeline.shaderStages = compileShader(std::string(standardMaterialVertexShader), std::string(standardMaterialFragmentShader));
			};

			void buildDynamicState() override
			{
				// TOD
				pipeline.dynamicStates.push_back(vk::DynamicState::eViewport);
				pipeline.dynamicStates.push_back(vk::DynamicState::eScissor);
				pipeline.dynamicStateInfo = vk::PipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags{},
																			   pipeline.dynamicStates);
			};
		};
	};

	class StandardMaterial2 : public core::Node
	{
		public:
		StandardMaterial2()
		{
		}

		//TODO change return type to std::expected
		bool initialize() override
		{
			defineNode("Standard Material");

			m_inputs.meshData = core::TypedAttribute<core::MeshData>::Builder("Mesh Data")
									.setReadable(false)
									.setWritable(true)
									.build();


			addAttribute(m_inputs.meshData);

			m_outputs.renderable = core::TypedAttribute<Renderable>::Builder("Renderable")
									   .setReadable(true)
									   .setWritable(false)
									   .build();

			addAttribute(m_outputs.renderable);

			return true;
		}

		bool compute() override
		{
			spdlog::info("StandardMaterial2::compute()");

			//Print the mesh data
			auto meshData = m_inputs.meshData.getValue();
			spdlog::info("Mesh Data: {}", meshData);

			Renderable renderable;
			renderable.m_vertexShader = standardMaterialVertexShader;
			renderable.m_fragmentShader = standardMaterialFragmentShader;
			renderable.m_meshData = meshData;


			m_outputs.renderable = renderable;

			return true;
		}

		struct Inputs
		{
			core::TypedInputHandler<core::MeshData> meshData;
		};

		struct Outputs
		{
			core::TypedOutputHandler<Renderable> renderable;
		};

		Inputs m_inputs;
		Outputs m_outputs;
	};

} // namespace st::renderer

#endif // ST_RENDERER_STANDARD_MATERIAL_HPP