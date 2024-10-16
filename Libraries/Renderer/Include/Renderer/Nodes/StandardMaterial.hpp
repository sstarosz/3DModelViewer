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

			/*
				Move the model 1 unit in x axis
				[1, 0, 0, 1]
				[0, 1, 0, 0]
				[0, 0, 1, 0]
				[0, 0, 0, 1]

				Move the model 2 units in y axis positive is down (?)
				[1, 0, 0, 0]
				[0, 1, 0, 2]
				[0, 0, 1, 0]
				[0, 0, 0, 1]
			*/

			mat4 debugModelMatrix = mat4(1.0);
			debugModelMatrix[3][2] = -1.0;

			mat4 debugViewMatrix;
			debugViewMatrix[0][0] = 1.0;
			debugViewMatrix[1][0] = 0.0;
			debugViewMatrix[2][0] = 0.0;
			debugViewMatrix[3][0] = 1.0;
			debugViewMatrix[0][1] = 0.0;
			debugViewMatrix[1][1] = 1.0;
			debugViewMatrix[2][1] = 0.0;
			debugViewMatrix[3][1] = 1.0;
			debugViewMatrix[0][2] = 0.0;
			debugViewMatrix[1][2] = 0.0;
			debugViewMatrix[2][2] = 1.0;
			debugViewMatrix[3][2] = 0.0;
			debugViewMatrix[0][3] = 0.0;
			debugViewMatrix[1][3] = 0.0;
			debugViewMatrix[2][3] = 0.0;
			debugViewMatrix[3][3] = 1.0;

			
			mat4 postViewMatrix = mat4(1.0);
			postViewMatrix[1][1] = -1.0;
			postViewMatrix[2][2] = -1.0;
			


			//mvp.proj * mvp.view * mvp.model * 
			vec4 worldPosition = mvp.model * vec4(inPosition, 1.0);
			vec4 viewPosition = mvp.view * worldPosition;
			//vec4 postViewPosition = postViewMatrix * viewPosition;
			vec4 projPosition = mvp.proj * viewPosition;
			gl_Position = mvp.proj * postViewMatrix * mvp.view * mvp.model * vec4(inPosition, 1.0);


			if(inPosition.y > 0.0)
			{
				outFragColor = vec3(0.0, 1.0, 1.0);
			}
			else
			{
				outFragColor = vec3(1.0, 0.0, 0.0);
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