#ifndef ST_RENDERER_STANDARD_MATERIAL_HPP
#define ST_RENDERER_STANDARD_MATERIAL_HPP

#include "Core/Node.hpp"
#include "Geometry/DataTypes/Mesh.hpp"
#include "Renderer/DataTypes/Renderable.hpp"
#include <string_view>

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

	class StandardMaterial : public core::Node
	{
	  public:
		StandardMaterial();

		// TODO change return type to std::expected
		bool initialize() override;

		bool compute() override;

		struct Inputs
		{
			core::TypedInputHandler<geometry::Mesh> meshData;
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