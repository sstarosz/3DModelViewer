#ifndef ST_GEOMETRY_LINE_HPP
#define ST_GEOMETRY_LINE_HPP

#include <Point3D.hpp>
#include <Geometry.hpp>

namespace st::geometry
{
    
    class Line : public Geometry
    {
    public:
        Line();
        Line(const Point3D& start, const Point3D& end);
        
        
    private:
        Point3D start;
        Point3D end;
    };

    
}   // namespace st::geometry

#endif //  ST_GEOMETRY_LINE_HPP