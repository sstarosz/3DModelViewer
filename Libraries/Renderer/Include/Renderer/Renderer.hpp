#ifndef ST_RENDERER_RENDERER_HPP
#define ST_RENDERER_RENDERER_HPP

#include "Core/Nodes/Node.hpp"
#include "Renderable.hpp"
#include "MaterialManager.hpp"

#include <memory>
#include <vulkan/vulkan.hpp>

namespace st::core
{
	class Scene;
}

namespace st::renderer
{

	class Renderer : public core::Node
	{
	  public:
		Renderer();
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept;
		
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept;

		bool initialize() override
		{
			//TODO cleanup
			defineNode("Renderer");


			m_input.renderable = core::TypedAttribute<Renderable>::Builder("Renderable")
									  .setReadable(false)
									  .setWritable(true)
									  .build();


			addAttribute(m_input.renderable);
			
			return true;
		}

        bool compute() override
		{
			spdlog::info("Renderer::compute()");
			//TODO cleanup
			return true;
		}



		void init();
		void render();
		void render(const core::Scene& scene);
		void embedScene(std::shared_ptr<core::Scene> scene);

		vk::Instance createInstance() const;
		void setSurface(vk::SurfaceKHR surface);

		void changeSwapchainExtent(uint32_t width, uint32_t height);

		const MaterialManager& getMaterialManager() const;


		struct Input
		{
			core::TypedInputHandler<Renderable> renderable;
		};

		Input m_input; 

	  private:
		class PrivateRenderer;
		std::unique_ptr<PrivateRenderer> m_privateRenderer;
	};

} // namespace st::renderer

#endif // ST_RENDERER_RENDERER_HPP