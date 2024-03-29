#ifndef ST_GEOMETRY_VERTEX_HPP
#define ST_GEOMETRY_VERTEX_HPP

#include <Eigen/Core>
#include <compare>
#include <ostream>

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

        friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
        {
            os << "Vertex(" << vertex.m_position.transpose() << ")";
            return os;
        }

      private:
        Eigen::Vector3f m_position;
    };

} // namespace st::geometry

#endif //  ST_GEOMETRY_VERTEX_HPP