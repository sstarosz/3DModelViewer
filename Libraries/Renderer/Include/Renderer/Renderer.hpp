#ifndef ST_RENDERER_RENDERER_HPP
#define ST_RENDERER_RENDERER_HPP

#include "Core/Node.hpp"
#include "Core/DataTypes/Camera.hpp"
#include "Renderer/DataTypes/Renderable.hpp"
#include "MaterialManager.hpp"

#include <memory>
#include <vulkan/vulkan.hpp>

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

			m_input.camera = core::TypedAttribute<core::Camera>::Builder("Camera")
								   .setReadable(false)
								   .setWritable(true)
								   .build();

			addAttribute(m_input.renderable);
			addAttribute(m_input.camera);

			init();
			
			return true;
		}

        bool compute() override
		{
			spdlog::info("Renderer::compute()");

			updateScene();
			
			//TODO cleanup
			return true;
		}



		void init();
		void render();

		vk::Instance getVulkanInstance() const;
		void setSurface(vk::SurfaceKHR surface);

		void changeSwapchainExtent(uint32_t width, uint32_t height);

		const MaterialManager& getMaterialManager() const;

		void updateScene();


		struct Input
		{
			core::TypedInputHandler<core::Camera> camera;
			core::TypedInputHandler<Renderable> renderable;
		};

		Input m_input; 

	  private:
		class PrivateRenderer;
		std::unique_ptr<PrivateRenderer> m_privateRenderer;
	};

} // namespace st::renderer

#endif // ST_RENDERER_RENDERER_HPP