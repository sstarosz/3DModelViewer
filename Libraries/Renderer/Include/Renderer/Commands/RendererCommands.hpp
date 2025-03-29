#ifndef ST_RENDERER_RENDERER_COMMANDS_HPP
#define ST_RENDERER_RENDERER_COMMANDS_HPP

#include "Core/Command.hpp"
#include "Core/ContentManager.hpp"
#include "Renderer/Renderer.hpp"

namespace st::renderer
{
	class CreateRendererCommand : public core::Command<std::shared_ptr<renderer::Renderer>>
	{
	  public:
		CreateRendererCommand(core::ContentManagerHandler contentManager, std::shared_ptr<core::Node> camera = nullptr);

		std::shared_ptr<renderer::Renderer> execute() override;
		void undo() override;

	  private:
		core::ContentManagerHandler m_contentManager;
		std::shared_ptr<renderer::Renderer> m_renderer;
    	std::shared_ptr<core::Node> m_camera;
	};
} // namespace st::renderer

#endif // ST_RENDERER_RENDERER_COMMANDS_HPP