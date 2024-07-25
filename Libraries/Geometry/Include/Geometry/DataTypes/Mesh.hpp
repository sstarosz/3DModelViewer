#ifndef ST_GEOMETRY_MESH_HPP
#define ST_GEOMETRY_MESH_HPP

#include <Eigen/Core>
#include <spdlog/spdlog.h>
#include <vector>

namespace st::geometry
{
	using FloatPointVector = std::vector<Eigen::Vector3f>;
	using IntVector = std::vector<uint32_t>;

	class Mesh
	{
	  public:
		Mesh();

		Mesh(const FloatPointVector& vertices,
			 const IntVector& indices);

		FloatPointVector getVertexPointList() const;

		IntVector getIndicesPointList() const;

	  private:
		std::vector<Eigen::Vector3f> m_vertices;
		std::vector<uint32_t> m_indices;
	};

} // namespace st::geometry

template <>
struct fmt::formatter<st::geometry::Mesh> : fmt::formatter<std::string>
{
	template <typename FormatContext>
	auto format(const st::geometry::Mesh& meshData, FormatContext& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "MeshData: vertices: {}, indices: {}", meshData.getVertexPointList().size(), meshData.getIndicesPointList().size());
	}
};

template <>
struct fmt::formatter<st::geometry::FloatPointVector> : fmt::formatter<std::string>
{
	template <typename FormatContext>
	auto format(const st::geometry::FloatPointVector& vertices, FormatContext& ctx) const -> decltype(ctx.out())
	{
		//Print all vertices
		fmt::format_to(ctx.out(), "Vertices (Count: {}):\n", vertices.size());
		for (const auto& vertex : vertices)
		{
			fmt::format_to(ctx.out(), "\tVertex: x: {}, y: {}, z: {}\n", vertex.x(), vertex.y(), vertex.z());
		}

		return ctx.out();
	}
};

template <>
struct fmt::formatter<st::geometry::IntVector> : fmt::formatter<std::string>
{
	template <typename FormatContext>
	auto format(const st::geometry::IntVector& indices, FormatContext& ctx) const -> decltype(ctx.out())
	{
		//Print all indices
		fmt::format_to(ctx.out(), "Indices (Count: {}):\n", indices.size());
		for (const auto& index : indices)
		{
			fmt::format_to(ctx.out(), "\tIndex: {}", index);
		}

		return ctx.out();
	}
};



#endif // ST_GEOMETRY_MESH_HPP
