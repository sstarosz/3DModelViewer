#include "Line.hpp"

namespace st::geometry
{

	Line::Line(const Eigen::Vector3f& start, const Eigen::Vector3f& end):
		start(start),
		end(end)
	{
	}

} // namespace st::geometry