#include "MaterialCommands.hpp"

namespace st::renderer
{

	CommandAssignMaterial::CommandAssignMaterial(core::ContentManagerHandler contentManager,
												 std::shared_ptr<renderer::StandardMaterial> material,
												 std::shared_ptr<core::Node> node) :
		m_contentManager(contentManager),
		m_material(material),
		m_node(node)
	{
	}


	void CommandAssignMaterial::execute()
	{
		std::shared_ptr<core::Attribute> sourceAttribute = nullptr;
		std::shared_ptr<core::Attribute> targetAttribute = nullptr;

		// Check if mesh node has Mesh output
        auto meshAttributes = m_node->getAttributes();
        for (auto& attribute : meshAttributes)
        {
            // Check if attribute is a TypedAttribute
            if (std::shared_ptr<core::TypedAttribute<geometry::Mesh>> meshData = std::dynamic_pointer_cast<core::TypedAttribute<geometry::Mesh>>(attribute))
            {
                std::println("Found Mesh");
                sourceAttribute = attribute;
            }
        }
		

		// Check if material has input for meshData
        auto materialAttributes = m_material->getAttributes();
        for (auto& attribute : materialAttributes)
        {
            if (std::shared_ptr<core::TypedAttribute<geometry::Mesh>> meshData = std::dynamic_pointer_cast<core::TypedAttribute<geometry::Mesh>>(attribute))
            {
                std::println("Found Mesh");
                targetAttribute = attribute;
            }
        }
		

		// Add connection
		m_contentManager->getMainNodeGraph().addConnection(m_node,
															sourceAttribute,
															m_material,
															targetAttribute);
	}

	void CommandAssignMaterial::undo()
	{
		// TODO implement
	}

} // namespace st::renderer