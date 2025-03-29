#include "StandardMaterialCommands.hpp"

namespace st::renderer
{
    CreateStandardMaterialCommand::CreateStandardMaterialCommand(core::ContentManagerHandler contentManager) :
        m_contentManager(contentManager)
    {
    }

    std::shared_ptr<StandardMaterial> CreateStandardMaterialCommand::execute()
    {
        m_standardMaterial = std::make_shared<renderer::StandardMaterial>();
        m_standardMaterial->initialize();
        m_contentManager->getMainNodeGraph().addNode(m_standardMaterial);

        return m_standardMaterial;
    }

    void CreateStandardMaterialCommand::undo()
    {
        // TODO implement
    }

} // namespace st::renderer