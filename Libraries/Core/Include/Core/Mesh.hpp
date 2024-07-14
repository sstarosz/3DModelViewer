#ifndef ST_CORE_MESH_HPP
#define ST_CORE_MESH_HPP

#include <Eigen/Core>
#include <vector>
#include <spdlog/spdlog.h>

namespace st::core
{
	using FloatPointVector = std::vector<Eigen::Vector3f>;
	using IntVector = std::vector<uint32_t>;

	class MeshData
	{
	  public:
		MeshData()
		{
		}

		MeshData(const FloatPointVector& vertices,
				 const IntVector& indices) :
			m_vertices(vertices),
			m_indices(indices)
		{
		}

		FloatPointVector getVertexPointList() const
		{
			return m_vertices;
		}

		IntVector getIndicesPointList() const
		{
			return m_indices;
		}


	  private:
		std::vector<Eigen::Vector3f> m_vertices;
		std::vector<uint32_t> m_indices;
	};
} // namespace st::core

template<>
struct fmt::formatter<st::core::MeshData>: fmt::formatter<std::string>
{
	template <typename FormatContext>
	auto format(const st::core::MeshData& meshData, FormatContext& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "MeshData: vertices: {}, indices: {}", meshData.getVertexPointList().size(), meshData.getIndicesPointList().size());
	}
};

#endif // ST_CORE_MESH_HPP
