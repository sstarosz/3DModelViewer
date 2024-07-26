#ifndef ST_RENDERER_RENDERER_COMMANDS_HPP
#define ST_RENDERER_RENDERER_COMMANDS_HPP

#include "Core/Command.hpp"
#include "Core/ContentManager.hpp"
#include "Renderer/Renderer.hpp"


namespace st::renderer
{
    class CreateRendererCommand : public core::Command
    {
      public:
        CreateRendererCommand(core::ContentManagerHandler contentManager);

        void execute() override;
        void undo() override;

        std::shared_ptr<renderer::Renderer> getResult();

        private:
		core::ContentManagerHandler m_contentManager;
		std::shared_ptr<renderer::Renderer> m_renderer; 
    };
} // namespace st::renderer

#endif // ST_RENDERER_RENDERER_COMMANDS_HPP