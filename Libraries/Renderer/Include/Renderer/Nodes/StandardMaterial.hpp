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

		layout(location = 0) out vec3 outFragColor;

		layout(set = 0, binding = 0) uniform MVP
		{
			mat4 model;
			mat4 view;
			mat4 proj;
		} mvp;

		bool compareMatrices(mat4 a, mat4 b)
		{
			const float epsilon = 0.0001;
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					if(abs(a[i][j] - b[i][j]) > epsilon)
					{
						return false;
					}
				}
			}

			return true;
		}

		bool compareVec3(vec3 a, vec3 b)
		{
			const float epsilon = 0.0001;
			for(int i = 0; i < 3; i++)
			{
				if(abs(a[i] - b[i]) > epsilon)
				{
					return false;
				}
			}

			return true;
		}
		

		void main()
		{
			/*
			[1, 0, 0, -0]
			[0, 1, 0, -0]
			[-0, -0, -1, 15]
			[0, 0, 0, 1]
			*/

			mat4 debugViewMatrix;
			debugViewMatrix[0] = vec4(1.0, 0.0, 0.0, 0.0);
			debugViewMatrix[1] = vec4(0.0, 1.0, 0.0, 0.0);
			debugViewMatrix[2] = vec4(0.0, 0.0, -1.0, 0.0);
			debugViewMatrix[3] = vec4(0.0, 0.0, 15.0, 1.0);


			mat4 viewMatrix = mat4(1.0);
			viewMatrix[2][2] = 1.0;
			viewMatrix[3][2] = -15.0;


			vec3 position = inPosition;

			//if(position.y > 0.0 && position.x > 0.0)
			//{
			//	position.y += 1.0;
			//}

			mat4 postViewRotation = mat4(1.0);
			postViewRotation[0][0] = 1.0;
			postViewRotation[1][1] = -1.0;
			postViewRotation[2][2] = -1.0;
			postViewRotation[3][3] = 1.0;

			vec4 transformedPosition = mvp.proj * mvp.view * mvp.model * vec4(position, 1.0);

			gl_Position = transformedPosition;



			if(transformedPosition.y > 0.0)
			{
				outFragColor = vec3(0.0, 1.0, 0.0);
			}
			else
			{
				outFragColor = vec3(1.0, 0.0, 0.0);
				// = vec3(mvp.view[0][0], mvp.view[1][1], mvp.view[2][2]);
			}
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