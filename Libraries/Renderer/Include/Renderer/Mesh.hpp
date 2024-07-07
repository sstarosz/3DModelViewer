#ifndef ST_RENDERER_MESH_HPP
#define ST_RENDERER_MESH_HPP

#include "Core/StObject.hpp"
#include "Core/Nodes/Node.hpp"
#include "Geometry/Geometry.hpp"

#include <Eigen/Core>
#include <vector>

namespace st::renderer
{
   using PointList = std::vector<Eigen::Vector3f>;
   using IndexList = std::vector<uint32_t>;


    class Mesh : public core::Node
    {
      public:
        Mesh(): core::Node()
        {
        }

        PointList getVertexPointList() const
        {
            return m_vertices;
        }

        IndexList getIndicesPointList() const
        {
            return m_indices;
        }

        private:
        std::vector<Eigen::Vector3f> m_vertices;
			  std::vector<uint32_t> m_indices;
        
    };

    class MeshFactory
    {
        public:

        std::weak_ptr<core::StObject> create(core::Node& node)
        {
            //TODO implement
            return std::weak_ptr<core::StObject>();
        }
    };

} // namespace st::renderer

#endif // ST_RENDERER_MESH_HPP

