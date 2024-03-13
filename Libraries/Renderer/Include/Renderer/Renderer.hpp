#ifndef ST_RENDERER_RENDERER_HPP
#define ST_RENDERER_RENDERER_HPP

#include <memory>
#include <vulkan/vulkan.hpp>

namespace st::core
{
	class Scene;
}

namespace st::renderer
{

	class Renderer
	{
	  public:
		Renderer();
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = default;
		
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = default;


		void init();
		void render();
		void render(const core::Scene& scene);

		vk::Instance createInstance() const;
		void setSurface(vk::SurfaceKHR surface);

		void changeSwapchainExtent(uint32_t width, uint32_t height);

	  private:
		class PrivateRenderer;
		std::unique_ptr<PrivateRenderer> m_privateRenderer;
	};

} // namespace st::renderer

#endif // ST_RENDERER_RENDERER_HPP