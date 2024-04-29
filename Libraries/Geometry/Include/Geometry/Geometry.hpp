#ifndef ST_GEOMETRY_GEOMETRY_HPP
#define ST_GEOMETRY_GEOMETRY_HPP

#include "Vertex.hpp"
#include "Core/StObject.hpp"
#include <vector>

namespace st::geometry
{

	/**
	 * @brief
	 *
	 */
	class Geometry
	{
		public:
			const std::vector<Vertex> getVertices() const
			{
				return m_vertices;
			}

			const std::vector<uint32_t> getIndices() const
			{
				return m_indices;
			
			}

		private:
			std::vector<Vertex> m_vertices;
			std::vector<uint32_t> m_indices;
	};

} // namespace st::geometry

#endif //  ST_GEOMETRY_GEOMETRY_HPP