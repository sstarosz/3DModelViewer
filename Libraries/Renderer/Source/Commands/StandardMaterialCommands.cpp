#include "StandardMaterialCommands.hpp"

namespace st::renderer
{
    CreateStandardMaterialCommand::CreateStandardMaterialCommand(core::ContentManagerHandler contentManager) :
        m_contentManager(contentManager)
    {
    }

    void CreateStandardMaterialCommand::execute()
    {
        m_standardMaterial = std::make_shared<renderer::StandardMaterial>();
        m_standardMaterial->initialize();
        m_contentManager->getMainNodeGraph()->addNode(m_standardMaterial);
    }

    void CreateStandardMaterialCommand::undo()
    {
        // TODO implement
    }

    std::shared_ptr<renderer::StandardMaterial> CreateStandardMaterialCommand::getResult()
    {
        return m_standardMaterial;
    }

} // namespace st::renderer