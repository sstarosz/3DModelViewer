#ifndef ST_RENDERER_STANDARD_MATERIAL_HPP
#define ST_RENDERER_STANDARD_MATERIAL_HPP

#include "GraphicsPiplineFactory.hpp"
#include "Material.hpp"
#include "Renderable.hpp"
#include "Core/NodeGraph.hpp"
#include "Core/Nodes/Node.hpp"
#include "Core/Mesh.hpp"

#include <string_view>
#include <string>

namespace st::renderer
{

	constexpr std::string_view standardMaterialVertexShader = R"(
        #version 330 core

        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";

	constexpr std::string_view standardMaterialFragmentShader = R"(
        #version 330 core

        out vec4 FragColor;

        in vec2 TexCoord;

        uniform sampler2D texture_diffuse1;

        void main()
        {
            FragColor = texture(texture_diffuse1, TexCoord);
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

	class StandardMaterial2 : public core::Node2
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