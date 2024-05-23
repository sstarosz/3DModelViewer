#ifndef ST_GEOMETRY_PLANE_HPP
#define ST_GEOMETRY_PLANE_HPP

#include "Core//NodeGraph.hpp"
#include "Core/Nodes/Node.hpp"
#include <Eigen/Core>
#include <vector>

namespace st::renderer
{

    /**
     * @brief 
     * 
     */
    class Plane : public core::Node
    {
        public:
            Plane()
            {
                
                m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(-0.5F, 0.0F, -0.5F)));
                m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(0.5F, 0.0F, -0.5F)));
                m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(0.5F, 0.0F, 0.5F)));
                m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(-0.5F, 0.0F, 0.5F)));

                m_indices.push_back(0);
                m_indices.push_back(1);
                m_indices.push_back(2);

                m_indices.push_back(2);
                m_indices.push_back(3);
                m_indices.push_back(0);
            }
 
            const std::vector<geometry::Vertex> getVertices() const
            {
                return m_vertices;
            }

            const std::vector<uint32_t> getIndices() const
            {
                return m_indices;
            }

        private:
            std::vector<geometry::Vertex> m_vertices;
            std::vector<uint32_t> m_indices;

    };

    class Plane2 : public core::Node2
    {
        public:
        Plane2()
        {
        }

        ~Plane2() override
        {
        }

       struct Inputs
       {
        
       };

        private:
            std::vector<geometry::Vertex> m_vertices;
            std::vector<uint32_t> m_indices;

    };

} // namespace st::renderer

#endif //  ST_GEOMETRY_GRID_HPP