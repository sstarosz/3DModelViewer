#ifndef ST_GEOMETRY_VERTEX_HPP
#define ST_GEOMETRY_VERTEX_HPP

#include <Eigen/Core>
#include <compare>


namespace st::geometry
{

    /**
     * @brief
     *
     */
    class Vertex
    {
      public:
        Vertex(const Eigen::Vector3f& position)
            : m_position(position)
        {
        }

        const Eigen::Vector3f& getPosition() const
        {
            return m_position;
        }

        auto operator<=>(const Vertex&) const = default;


      private:
        Eigen::Vector3f m_position;
    };

} // namespace st::geometry

#endif //  ST_GEOMETRY_VERTEX_HPP