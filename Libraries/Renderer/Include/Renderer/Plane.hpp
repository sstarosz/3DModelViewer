#ifndef ST_GEOMETRY_PLANE_HPP
#define ST_GEOMETRY_PLANE_HPP

#include "Core/Mesh.hpp"
#include "Core/NodeGraph.hpp"
#include "Core/Nodes/Node.hpp"
#include <Eigen/Core>
#include <vector>

namespace st::renderer
{

	/**
	 * @brief
	 *
	 */
	class Plane : public core::Node
	{
	  public:
		Plane()
		{

			m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(-0.5F, 0.0F, -0.5F)));
			m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(0.5F, 0.0F, -0.5F)));
			m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(0.5F, 0.0F, 0.5F)));
			m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(-0.5F, 0.0F, 0.5F)));

			m_indices.push_back(0);
			m_indices.push_back(1);
			m_indices.push_back(2);

			m_indices.push_back(2);
			m_indices.push_back(3);
			m_indices.push_back(0);
		}

		const std::vector<geometry::Vertex> getVertices() const
		{
			return m_vertices;
		}

		const std::vector<uint32_t> getIndices() const
		{
			return m_indices;
		}

	  private:
		std::vector<geometry::Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
	};

	class Plane2 : public core::Node2
	{
	  public:
		Plane2()
		{
		}

		// TODO change return type to std::expected
		bool initialize() override
		{
			defineNode("Plane");

			m_inputs.width = core::NumericAttribute<float>::Builder("Width", 1.0F)
								 .setReadable(true)
								 .setWritable(true)
								 .setLimits(0.001F, 100000.0F)
								 .setSoftLimits(0.001F, 100.0F)
								 .build();

			addAttribute(m_inputs.width);

			m_inputs.height = core::NumericAttribute<float>::Builder("Height", 1.0F)
								  .setReadable(true)
								  .setWritable(true)
								  .setLimits(0.001F, 100000.0F)
								  .setSoftLimits(0.001F, 100.0F)
								  .build();

			addAttribute(m_inputs.height);

			m_inputs.subdivisionWidth = core::NumericAttribute<uint32_t>::Builder("Subdivision Width", 1)
											.setReadable(true)
											.setWritable(true)
											.setLimits(1, 100000)
											.setSoftLimits(1, 100)
											.build();

			addAttribute(m_inputs.subdivisionWidth);

			m_inputs.subdivisionHeight = core::NumericAttribute<uint32_t>::Builder("Subdivision Height", 1)
											 .setReadable(true)
											 .setWritable(true)
											 .setLimits(1, 100000)
											 .setSoftLimits(1, 100)
											 .build();

			addAttribute(m_inputs.subdivisionHeight);


			m_outputs.mesh = core::TypedAttribute<core::MeshData>::Builder("Mesh")
								 .setReadable(true)
								 .setWritable(false)
								 .build();


            addAttribute(m_outputs.mesh);

			return true;
		}

		bool compute() override
		{
			// TODO implement
			// for(int i = 0; i < m_inputs.subdivisionWidth; i++)
			//{
			//     for(int j = 0; j < m_inputs.subdivisionHeight; j++)
			//     {
			core::FloatPointVector vertices;
			core::IntVector indices;

			vertices.push_back(Eigen::Vector3f(-0.5F, 0.0F, -0.5F));
			vertices.push_back(Eigen::Vector3f(0.5F, 0.0F, -0.5F));
			vertices.push_back(Eigen::Vector3f(0.5F, 0.0F, 0.5F));
			vertices.push_back(Eigen::Vector3f(-0.5F, 0.0F, 0.5F));

			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);

			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(0);
			//    }
			//}

			m_outputs.mesh = core::MeshData(vertices, indices);

			return true;
		}

		struct Inputs
		{
			// core::Input<float> width;
			core::NumericInputHandler<float> width;
			core::NumericInputHandler<float> height;

			core::NumericInputHandler<uint32_t> subdivisionWidth;
			core::NumericInputHandler<uint32_t> subdivisionHeight;
		};

		struct Outputs
		{
			core::TypedOutputHandler<core::MeshData> mesh;
		};

	  private:
		Inputs m_inputs;
		Outputs m_outputs;
	};

	// RegisterNode<Plane2> plane2("Plane2");

} // namespace st::renderer

#endif //  ST_GEOMETRY_GRID_HPP