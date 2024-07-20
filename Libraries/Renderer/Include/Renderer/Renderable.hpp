#ifndef ST_RENDERER_RENDERABLE_HPP
#define ST_RENDERER_RENDERABLE_HPP

#include <vulkan/vulkan.hpp>

#include "Geometry/DataTypes/Mesh.hpp"

namespace st::renderer
{

	/**
	 * @brief
	 *
	 */
	class Renderable
	{

	  public:
		struct Material
		{
			vk::Pipeline m_pipeline;
			vk::PipelineLayout m_pipelineLayout;
			vk::DescriptorSet m_descriptorSet;
		};

		Renderable()
		{
		}

		Renderable(const std::string& name) :
			m_name(name)
		{
		}

	  public:
		std::string m_name;

		std::string m_vertexShader;
		std::string m_fragmentShader;

		geometry::Mesh m_meshData;
	};

} // namespace st::renderer

template <>
struct fmt::formatter<st::renderer::Renderable> : fmt::formatter<std::string>
{
	template <typename FormatContext>
	auto format(const st::renderer::Renderable& renderable, FormatContext& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "Renderable: {}", renderable.m_name);
	}
};

#endif //  ST_RENDERER_RENDERABLE_HPP