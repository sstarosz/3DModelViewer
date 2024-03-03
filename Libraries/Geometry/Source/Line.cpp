#include "Line.hpp"

namespace st::geometry
{

	Line::Line() :
		start(),
		end()
	{
		addAttribute("vertexPosition", Attribute::Type::eFloat, start);
	}

	Line::Line(const Point3D& start, const Point3D& end) :
		start(start),
		end(end)
	{
	}

} // namespace st::geometry