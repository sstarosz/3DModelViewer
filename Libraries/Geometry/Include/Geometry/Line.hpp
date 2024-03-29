#ifndef ST_GEOMETRY_LINE_HPP
#define ST_GEOMETRY_LINE_HPP

#include "Geometry.hpp"
#include "Vertex.hpp"

namespace st::geometry
{

	class Line : public Geometry
	{
	  public:
		Line(const Eigen::Vector3f& start, const Eigen::Vector3f& end);

		const std::vector<Vertex> getVertices() const override
		{
			return {start, end};
		}

		const std::vector<uint32_t> getIndices() const override
		{
			return {0, 1};
		}

	  private:
		Vertex start;
		Vertex end;
	};

} // namespace st::geometry

#endif //  ST_GEOMETRY_LINE_HPP