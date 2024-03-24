#ifndef ST_GEOMETRY_GRID_HPP
#define ST_GEOMETRY_GRID_HPP

#include "Geometry.hpp"
#include "Line.hpp"


namespace st::geometry
{

    /**
     * @brief 
     * 
     */
    class Grid : public Geometry
    {
        public:
            Grid(float lengthAndWidth, float division)
            {
                assert(division >= 1 && "Division must be greater or equal to 1");

                std::vector<Line> lines;
                
                //From -lengthAndWidth/2 to lengthAndWidth/2
                float half = lengthAndWidth / 2.0F;
                float step = lengthAndWidth / division;
                for (float i = -half; i < half; i += step)
                {
                    m_lines.push_back(Eigen::Vector3f(i, 0, -half));
                    m_lines.push_back(Eigen::Vector3f(i, 0, half));

                    m_lines.push_back(Eigen::Vector3f(-half, 0, i));
                    m_lines.push_back(Eigen::Vector3f(half, 0, i));
                }
            }
 
            const std::vector<Vertex>& getVertices() const override
            {
                return m_lines;
            }

        private:
            float m_width;
            float m_height;
            float m_depth;
            float m_cellSize;

            std::vector<Vertex> m_lines;
    };

} // namespace st::geometry

#endif //  ST_GEOMETRY_GRID_HPP