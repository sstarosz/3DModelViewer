#ifndef ST_RENDERER_STANDARDMATERIAL_COMMANDS_HPP
#define ST_RENDERER_STANDARDMATERIAL_COMMANDS_HPP

#include "Core/Command.hpp"
#include "Core/ContentManager.hpp"
#include "Renderer/Nodes/StandardMaterial.hpp"

namespace st::renderer
{
    class CreateStandardMaterialCommand : public core::Command
    {
        public:
        CreateStandardMaterialCommand(core::ContentManagerHandler contentManager);
        void execute() override;
        void undo() override;
        std::shared_ptr<StandardMaterial> getResult();

      private:
        core::ContentManagerHandler m_contentManager;
        std::shared_ptr<StandardMaterial> m_standardMaterial;
    };

} // namespace st::renderer

#endif // ST_RENDERER_STANDARDMATERIAL_COMMANDS_HPP