#include "StandardMaterial.hpp"

#include "GraphicsPiplineFactory.hpp"
#include "Material.hpp"
#include "Renderable.hpp"
#include "Core/Node.hpp"
#include "Geometry/DataTypes/Mesh.hpp"

#include <string_view>
#include <string>

namespace st::renderer
{

		StandardMaterial::StandardMaterial()
		{
		}

		//TODO change return type to std::expected
		bool StandardMaterial::initialize()
		{
			defineNode("Standard Material");

			m_inputs.meshData = core::TypedAttribute<geometry::Mesh>::Builder("Mesh Data")
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

		bool StandardMaterial::compute()
		{
			spdlog::info("StandardMaterial2::compute()");

			//Print the mesh data
			auto meshData = m_inputs.meshData.getData();
			spdlog::info("Mesh Data: {}", meshData);

			Renderable renderable;
			renderable.m_vertexShader = standardMaterialVertexShader;
			renderable.m_fragmentShader = standardMaterialFragmentShader;
			renderable.m_meshData = meshData;


			m_outputs.renderable = renderable;

			return true;
		}


} // namespace st::renderer
