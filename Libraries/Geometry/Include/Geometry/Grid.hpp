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
            Grid(float lengthAndWidth, uint32_t division)
            {
                assert(division >= 1 && "Division must be greater or equal to 1");
 
                // From -lengthAndWidth/2 to lengthAndWidth/2
                float xStart = -lengthAndWidth / 2;
                float step = lengthAndWidth / division;
                for (uint32_t i = 0; i <= division; i++) 
                {
                    for (uint32_t j = 0; j <= division; j++) 
                    {
                        // Add a vertex at each grid point
                        m_vertices.push_back(Vertex(Eigen::Vector3f(xStart + static_cast<float>(i) * step, 0.0F, xStart + static_cast<float>(j) * step)));

                        // Add lines between the vertices
                        if (i < division)
                        {
                            m_indices.push_back(i * (division + 1) + j);
                            m_indices.push_back(i + 1 * (division + 1) + j);
                        }
                        if (j < division)
                        {
                            m_indices.push_back(i * (division + 1) + j);
                            m_indices.push_back(i * (division + 1) + j + 1);
                        }
                    }
                }

            }
 
            const std::vector<Vertex> getVertices() const override
            {
                return m_vertices;
            }

            const std::vector<uint32_t> getIndices() const override
            {
                return m_indices;
            }

        private:
            float m_width;
            float m_height;
            float m_depth;
            float m_cellSize;

            std::vector<Vertex> m_vertices;
            std::vector<uint32_t> m_indices;

    };

} // namespace st::geometry

#endif //  ST_GEOMETRY_GRID_HPP