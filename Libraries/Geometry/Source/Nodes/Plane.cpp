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
				//Start from the bottom left corner
				float x = widthStartPos + rowIdx * stepWidth;
				float y = heightStartPos + colIdx * stepHeight;
				float z = 0.0F;

				vertices.push_back(Eigen::Vector3f(x, y, z));
			}
		}


		geometry::IntVector indices;
		//Iterate over faces
		for (uint32_t colIdx = 0; colIdx < m_inputs.subdivisionHeight; colIdx++)
		{
			for (uint32_t rowIdx = 0; rowIdx < m_inputs.subdivisionWidth; rowIdx++)
			{
				uint32_t bottomLeft = colIdx + rowIdx * numVerticesWidth;
				uint32_t bottomRight = bottomLeft + 1;
				uint32_t topLeft = bottomLeft + numVerticesWidth;
				uint32_t topRight = topLeft + 1;

				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
				indices.push_back(topRight);

				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topLeft);
			}
		}


		geometry::Mesh meshData(vertices, indices);
		spdlog::info("Mesh Data: {}", meshData);
		spdlog::info("Vertices: {}", meshData.getVertexPointList());
		spdlog::info("Indices: {}", meshData.getIndicesPointList());
		m_outputs.mesh = meshData;


		m_outputs.mesh = generateDebugMesh();
		return true;
	}


	geometry::Mesh Plane::generateDebugMesh()
	{
		geometry::FloatPointVector vertices = {
			Eigen::Vector3f(0.500000   ,0.000000   ,-0.075235),
			Eigen::Vector3f(0.500000   ,0.000000   ,-0.116125),
			Eigen::Vector3f(-0.035274   ,0.000000   ,0.500000),
			Eigen::Vector3f(-0.001982   ,0.000000   ,0.500000),
			Eigen::Vector3f(0.500000   ,0.109860   ,-0.037807),
			Eigen::Vector3f(-0.500000   ,0.500000   ,0.500000),
			Eigen::Vector3f(0.500000   ,0.500000   ,0.500000),
			Eigen::Vector3f(0.500000   ,0.500000   ,-0.500000),
			Eigen::Vector3f(-0.500000   ,0.500000   ,-0.500000),
			Eigen::Vector3f(0.500000   ,0.099240   ,-0.008388),
			Eigen::Vector3f(-0.001982   ,0.106532   ,0.500000),
			Eigen::Vector3f(0.500000   ,0.000000   ,0.055567),
			Eigen::Vector3f(0.500000   ,0.000000   ,0.022593),
			Eigen::Vector3f(-0.035274   ,0.234627   ,0.500000),
			Eigen::Vector3f(0.500000   ,0.121237   ,-0.057804),
			Eigen::Vector3f(0.500000   ,0.128438   ,-0.065914),
			Eigen::Vector3f(0.500000   ,0.136653   ,-0.072765),
			Eigen::Vector3f(0.500000   ,0.115042   ,-0.048433),
			Eigen::Vector3f(0.500000   ,0.145679   ,-0.078209),
			Eigen::Vector3f(0.500000   ,0.155321   ,-0.082099),
			Eigen::Vector3f(0.500000   ,0.099240   ,0.022593),
			Eigen::Vector3f(0.500000   ,0.165575   ,-0.084430),
			Eigen::Vector3f(0.500000   ,0.176445   ,-0.085209),
			Eigen::Vector3f(0.500000   ,0.189792   ,-0.084086),
			Eigen::Vector3f(0.500000   ,0.201455   ,-0.080713),
			Eigen::Vector3f(0.080138   ,0.106532   ,0.500000),
			Eigen::Vector3f(0.500000   ,0.211432   ,-0.075089),
			Eigen::Vector3f(-0.001982   ,0.131104   ,0.500000),
			Eigen::Vector3f(-0.001982   ,0.209737   ,0.500000),
			Eigen::Vector3f(0.080138   ,0.131104   ,0.500000),
			Eigen::Vector3f(0.500000   ,0.219724   ,-0.067216),
			Eigen::Vector3f(0.500000   ,0.226244   ,-0.057232),
			Eigen::Vector3f(0.500000   ,0.234627   ,0.055567),
			Eigen::Vector3f(0.500000   ,0.230903   ,-0.045278),
			Eigen::Vector3f(0.500000   ,0.233695   ,-0.031352),
			Eigen::Vector3f(0.500000   ,0.234627   ,-0.015456),
			Eigen::Vector3f(0.095833   ,0.209737   ,0.500000),
			Eigen::Vector3f(0.095833   ,0.234627   ,0.500000),
			Eigen::Vector3f(-0.500000   ,-0.500000   ,0.500000),
			Eigen::Vector3f(0.500000   ,-0.500000   ,0.500000),
			Eigen::Vector3f(0.500000   ,-0.500000   ,-0.500000),
			Eigen::Vector3f(-0.019657   ,0.234627   ,-0.500000),
			Eigen::Vector3f(0.041219   ,0.234627   ,-0.500000),
			Eigen::Vector3f(-0.036229   ,0.233795   ,-0.500000),
			Eigen::Vector3f(-0.500000   ,0.234627   ,-0.005865),
			Eigen::Vector3f(-0.500000   ,0.234627   ,-0.039157),
			Eigen::Vector3f(-0.050748   ,0.231299   ,-0.500000),
			Eigen::Vector3f(-0.063219   ,0.227136   ,-0.500000),
			Eigen::Vector3f(0.041219   ,0.000000   ,-0.500000),
			Eigen::Vector3f(-0.073638   ,0.221312   ,-0.500000),
			Eigen::Vector3f(-0.081856   ,0.213919   ,-0.500000),
			Eigen::Vector3f(-0.087726   ,0.205062   ,-0.500000),
			Eigen::Vector3f(-0.091248   ,0.194737   ,-0.500000),
			Eigen::Vector3f(-0.500000   ,0.024890   ,-0.005865),
			Eigen::Vector3f(-0.092424   ,0.182945   ,-0.500000),
			Eigen::Vector3f(-0.500000   ,0.000000   ,0.099716),
			Eigen::Vector3f(-0.500000   ,0.024890   ,0.099716),
			Eigen::Vector3f(-0.091641   ,0.173078   ,-0.500000),
			Eigen::Vector3f(-0.089291   ,0.163763   ,-0.500000),
			Eigen::Vector3f(-0.044229   ,0.122227   ,-0.500000),
			Eigen::Vector3f(-0.055138   ,0.127032   ,-0.500000),
			Eigen::Vector3f(-0.085378   ,0.155003   ,-0.500000),
			Eigen::Vector3f(-0.064719   ,0.132731   ,-0.500000),
			Eigen::Vector3f(-0.079898   ,0.146799   ,-0.500000),
			Eigen::Vector3f(-0.072972   ,0.139318   ,-0.500000),
			Eigen::Vector3f(-0.069407   ,0.111656   ,-0.500000),
			Eigen::Vector3f(-0.500000   ,0.000000   ,-0.039157),
			Eigen::Vector3f(-0.087388   ,0.097536   ,-0.500000),
			Eigen::Vector3f(-0.027901   ,0.000000   ,-0.500000),
			Eigen::Vector3f(-0.098180   ,0.079870   ,-0.500000),
			Eigen::Vector3f(-0.500000   ,-0.500000   ,-0.500000),
			Eigen::Vector3f(-0.101778   ,0.058656   ,-0.500000),
			Eigen::Vector3f(-0.039365   ,0.000305   ,-0.500000),
			Eigen::Vector3f(-0.049502   ,0.001227   ,-0.500000),
			Eigen::Vector3f(-0.100985   ,0.048640   ,-0.500000),
			Eigen::Vector3f(-0.058310   ,0.002763   ,-0.500000),
			Eigen::Vector3f(-0.098606   ,0.039195   ,-0.500000),
			Eigen::Vector3f(-0.065791   ,0.004912   ,-0.500000),
			Eigen::Vector3f(-0.094641   ,0.030328   ,-0.500000),
			Eigen::Vector3f(-0.072359   ,0.007825   ,-0.500000),
			Eigen::Vector3f(-0.078433   ,0.011649   ,-0.500000),
			Eigen::Vector3f(-0.089093   ,0.022036   ,-0.500000),
			Eigen::Vector3f(-0.084011   ,0.016386   ,-0.500000),
		};

		geometry::IntVector indices =
		{
			6-1   ,7-1   ,9-1   ,9-1   ,7-1   ,8-1   ,39-1   ,3-1   ,6-1   ,3-1   ,14-1   ,6-1   ,37-1   ,7-1   ,38-1   ,6-1   ,14-1   ,7-1   ,14-1   ,38-1   ,7-1   ,39-1   ,40-1   ,3-1   ,3-1   ,40-1   ,4-1   ,7-1   ,37-1   ,40-1   ,29-1   ,28-1   ,37-1   ,28-1   ,30-1   ,37-1   ,37-1   ,30-1   ,40-1   ,30-1   ,26-1   ,40-1   ,11-1   ,4-1   ,26-1   ,26-1   ,4-1   ,40-1   ,31-1   ,27-1   ,8-1   ,5-1   ,2-1   ,18-1   ,18-1   ,2-1   ,15-1   ,27-1   ,25-1   ,8-1   ,15-1   ,2-1   ,16-1   ,25-1   ,24-1   ,8-1   ,16-1   ,2-1   ,17-1   ,41-1   ,8-1   ,2-1   ,8-1   ,24-1   ,2-1   ,24-1   ,23-1   ,2-1   ,23-1   ,22-1   ,2-1   ,22-1   ,20-1   ,2-1   ,20-1   ,19-1   ,2-1   ,19-1   ,17-1   ,2-1   ,41-1   ,2-1   ,40-1   ,2-1   ,1-1   ,40-1   ,21-1   ,13-1   ,10-1   ,10-1   ,13-1   ,1-1   ,1-1   ,13-1   ,40-1   ,13-1   ,12-1   ,40-1   ,40-1   ,12-1   ,7-1   ,12-1   ,33-1   ,7-1   ,31-1   ,8-1   ,32-1   ,8-1   ,7-1   ,32-1   ,32-1   ,7-1   ,34-1   ,34-1   ,7-1   ,35-1   ,35-1   ,7-1   ,36-1   ,33-1   ,36-1   ,7-1   ,71-1   ,41-1   ,39-1   ,39-1   ,41-1   ,40-1   ,6-1   ,57-1   ,39-1   ,57-1   ,56-1   ,39-1   ,71-1   ,39-1   ,67-1   ,56-1   ,67-1   ,39-1   ,71-1   ,67-1   ,9-1   ,67-1   ,46-1   ,9-1   ,9-1   ,46-1   ,6-1   ,46-1   ,45-1   ,6-1   ,54-1   ,57-1   ,45-1   ,45-1   ,57-1   ,6-1   ,71-1   ,9-1   ,58-1   ,71-1   ,69-1   ,41-1   ,69-1   ,49-1   ,41-1   ,41-1   ,49-1   ,8-1   ,49-1   ,43-1   ,8-1   ,58-1   ,9-1   ,55-1   ,55-1   ,9-1   ,53-1   ,53-1   ,9-1   ,52-1   ,52-1   ,9-1   ,51-1   ,51-1   ,9-1   ,50-1   ,50-1   ,9-1   ,48-1   ,9-1   ,8-1   ,48-1   ,48-1   ,8-1   ,47-1   ,47-1   ,8-1   ,44-1   ,44-1   ,8-1   ,42-1   ,8-1   ,43-1   ,42-1   ,58-1   ,59-1   ,71-1   ,60-1   ,66-1   ,61-1   ,69-1   ,71-1   ,73-1   ,73-1   ,71-1   ,74-1   ,59-1   ,62-1   ,71-1   ,74-1   ,71-1   ,76-1   ,76-1   ,71-1   ,78-1   ,62-1   ,64-1   ,71-1   ,78-1   ,71-1   ,80-1   ,64-1   ,65-1   ,71-1   ,61-1   ,66-1   ,63-1   ,80-1   ,71-1   ,81-1   ,65-1   ,63-1   ,71-1   ,81-1   ,71-1   ,83-1   ,83-1   ,71-1   ,82-1   ,82-1   ,71-1   ,79-1   ,79-1   ,71-1   ,77-1   ,77-1   ,71-1   ,75-1   ,66-1   ,68-1   ,63-1   ,63-1   ,68-1   ,71-1   ,68-1   ,70-1   ,71-1   ,70-1   ,72-1   ,71-1   ,72-1   ,75-1   ,71
		};

		return geometry::Mesh(vertices, indices);
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
