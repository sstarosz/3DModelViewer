#include "Mesh.hpp"

namespace st::geometry
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const FloatPointVector& vertices,
					   const IntVector& indices) :
		m_vertices(vertices),
		m_indices(indices)
	{
	}

	FloatPointVector Mesh::getVertexPointList() const
	{
		return m_vertices;
	}

	IntVector Mesh::getIndicesPointList() const
	{
		return m_indices;
	}

} // namespace st::geometry