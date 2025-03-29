#ifndef ST_RENDERER_MATERIAL_COMMANDS_HPP
#define ST_RENDERER_MATERIAL_COMMANDS_HPP

#include "Core/Command.hpp"
#include "Core/ContentManager.hpp"
#include "Renderer/Nodes/StandardMaterial.hpp"

namespace st::renderer
{
    class CommandAssignMaterial : public core::Command<void>
    {
      public:
		CommandAssignMaterial(core::ContentManagerHandler contentManager,
							  std::shared_ptr<renderer::StandardMaterial> material,
							  std::shared_ptr<core::Node> node);

		void execute() override;
        void undo() override;


        private:
		core::ContentManagerHandler m_contentManager;
		std::shared_ptr<renderer::StandardMaterial> m_material;
		std::shared_ptr<core::Node> m_node;
    };
} // namespace st::renderer

#endif // ST_RENDERER_MATERIAL_COMMANDS_HPP