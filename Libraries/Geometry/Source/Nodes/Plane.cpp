#include "Plane.hpp"
#include <Eigen/Core>

namespace st::geometry
{
	/*-----------------------------Plane---------------------------------*/

	// TODO change return type to std::expected
	bool Plane::initialize()
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

		m_outputs.mesh = core::TypedAttribute<geometry::Mesh>::Builder("Mesh")
							 .setReadable(true)
							 .setWritable(false)
							 .build();

		addAttribute(m_outputs.mesh);

		return true;
	}

	bool Plane::compute()
	{
		spdlog::info("Plane::compute()");
		// ! Plane is placed in the XY plane, and in the center of the world space

		//The plane is subdivided into a grid of vertices
		float widthStartPos = 0.0F - m_inputs.width / 2.0F;
		float heightStartPos = 0.0F - m_inputs.height / 2.0F;

		//The step width and height between each vertex
		float stepWidth = m_inputs.width / m_inputs.subdivisionWidth;
		float stepHeight = m_inputs.height / m_inputs.subdivisionHeight;

		//The number of vertices in the width and height
		uint32_t numVerticesWidth = m_inputs.subdivisionWidth + 1;
		uint32_t numVerticesHeight = m_inputs.subdivisionHeight + 1;


		geometry::FloatPointVector vertices;
		for (uint32_t colIdx = 0; colIdx < numVerticesHeight; colIdx++)
		{
			for (uint32_t rowIdx = 0; rowIdx < numVerticesWidth; rowIdx++)
			{
				float x = widthStartPos + rowIdx * stepWidth;
				float y = heightStartPos + colIdx * stepHeight;
				float z = 0.0F;

				vertices.push_back(Eigen::Vector3f(x, y, z));
			}
		}


		geometry::IntVector indices;
		//Iterate over faces
		for (uint32_t rowIdx = 0; rowIdx < m_inputs.subdivisionWidth; rowIdx++)
		{
			for (uint32_t colIdx = 0; colIdx < m_inputs.subdivisionHeight; colIdx++)
			{
				uint32_t bottomLeft = rowIdx * numVerticesHeight + colIdx;
				uint32_t bottomRight = bottomLeft + 1;
				uint32_t topLeft = (rowIdx + 1) * numVerticesHeight + colIdx;
				uint32_t topRight = topLeft + 1;

				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
				indices.push_back(topRight);

				indices.push_back(topRight);
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);

			}
		}


		geometry::Mesh meshData(vertices, indices);
		spdlog::info("Mesh Data: {}", meshData);
		spdlog::info("Vertices: {}", meshData.getVertexPointList());
		spdlog::info("Indices: {}", meshData.getIndicesPointList());
		m_outputs.mesh = meshData;
		return true;
	}

	/*----------------------*/
	/*-------Setters--------*/
	/*----------------------*/
	void Plane::setWidth(float width)
	{
		m_inputs.width = width;
	}

	void Plane::setHeight(float height)
	{
		m_inputs.height = height;
	}

	void Plane::setSubdivisionWidth(uint32_t subdivisionWidth)
	{
		m_inputs.subdivisionWidth = subdivisionWidth;
	}

	void Plane::setSubdivisionHeight(uint32_t subdivisionHeight)
	{
		m_inputs.subdivisionHeight = subdivisionHeight;
	}

	/*----------------------*/
	/*-------Getters--------*/
	/*----------------------*/
	float Plane::getWidth() const
	{
		return m_inputs.width;
	}

	float Plane::getHeight() const
	{
		return m_inputs.height;
	}

	uint32_t Plane::getSubdivisionWidth() const
	{
		return m_inputs.subdivisionWidth;
	}

	uint32_t Plane::getSubdivisionHeight() const
	{
		return m_inputs.subdivisionHeight;
	}

	geometry::Mesh Plane::getMesh() const
	{
		return *m_outputs.mesh.getData();
	}

} // namespace st::geometry
