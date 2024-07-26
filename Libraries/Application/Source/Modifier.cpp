#include "Modifier.hpp"

#include "Geometry/Commands/PlaneCommands.hpp"
#include "Renderer/Commands/MaterialCommands.hpp"
#include "Renderer/Commands/StandardMaterialCommands.hpp"

namespace st::application
{

	/*---------------------------------*/
	/*--------MaterialModifier---------*/
	/*---------------------------------*/
	MaterialModifier::MaterialModifier(ModifyContext context, core::CommandManagerHandler commandManager) :
		m_context(context),
		m_commandManager(commandManager)
	{
	}

	void MaterialModifier::assign(std::weak_ptr<renderer::StandardMaterial> material)
	{
		// TODO refactor
		std::shared_ptr<renderer::StandardMaterial> materialPtr = material.lock();
		std::shared_ptr<core::Node> nodePtr = m_context.selectedNode.lock();
		std::unique_ptr<renderer::CommandAssignMaterial> command = std::make_unique<renderer::CommandAssignMaterial>(m_context.m_contentManager, materialPtr, nodePtr);
		m_commandManager->execute(std::move(command));
	}


	/*---------------------------------*/
	/*------------Modifier-------------*/
	/*---------------------------------*/
	Modifier::Modifier(ModifyContext context,
					   core::CommandManagerHandler commandManager) :
		m_context(context),
		m_commandManager(commandManager)
	{
	}

	void Modifier::transform()
	{
		// TODO refactor
	}

	MaterialModifier Modifier::material()
	{
		// TODO refactor
		//  Throw exception if node can't have material
		if (auto node = m_context.selectedNode.lock())
		{
			// Throw if node is not of type plane
			if (!std::dynamic_pointer_cast<geometry::Plane>(node))
			{
				throw std::runtime_error("Node is not of type Plane");
			}
		}

		MaterialModifier materialManager{m_context, m_commandManager};
		return materialManager;
	}

} // namespace st::application