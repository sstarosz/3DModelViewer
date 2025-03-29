#ifndef ST_RENDERER_STANDARDMATERIAL_COMMANDS_HPP
#define ST_RENDERER_STANDARDMATERIAL_COMMANDS_HPP

#include "Core/Command.hpp"
#include "Core/ContentManager.hpp"
#include "Renderer/Nodes/StandardMaterial.hpp"

namespace st::renderer
{
    class CreateStandardMaterialCommand : public core::Command<std::shared_ptr<StandardMaterial>>
    {
        public:
        CreateStandardMaterialCommand(core::ContentManagerHandler contentManager);
        std::shared_ptr<StandardMaterial> execute() override;
        void undo() override;

      private:
        core::ContentManagerHandler m_contentManager;
        std::shared_ptr<StandardMaterial> m_standardMaterial;
    };

} // namespace st::renderer

#endif // ST_RENDERER_STANDARDMATERIAL_COMMANDS_HPP