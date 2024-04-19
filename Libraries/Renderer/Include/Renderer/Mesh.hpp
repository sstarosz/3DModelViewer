#ifndef ST_RENDERER_MESH_HPP
#define ST_RENDERER_MESH_HPP

#include "Core/StObject.hpp"

namespace st::renderer
{
    class Mesh : public core::StObject
    {
      public:
        Mesh():
            core::StObject()
        {

        }

        ~Mesh()
        {

        }

        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&)
        {

        }

        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&)
        {
          
        }

      private:
        class PrivateMesh
        {
        };
    };

} // namespace st::renderer

#endif // ST_RENDERER_MESH_HPP

