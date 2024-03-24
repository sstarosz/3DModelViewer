#ifndef ST_GEOMETRY_GEOMETRY_HPP
#define ST_GEOMETRY_GEOMETRY_HPP

#include "Vertex.hpp"
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
		virtual const std::vector<Vertex>& getVertices() const = 0;
	};

} // namespace st::geometry

#endif //  ST_GEOMETRY_GEOMETRY_HPP