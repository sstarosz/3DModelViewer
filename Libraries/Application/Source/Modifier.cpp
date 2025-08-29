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

	void MaterialModifier::assign(std::shared_ptr<renderer::StandardMaterial> material)
	{
		std::unique_ptr<renderer::CommandAssignMaterial> command = std::make_unique<renderer::CommandAssignMaterial>(m_context.m_contentManager, material, m_context.selectedNode);
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
		// TODO Throw exception if node can't have material
		// TODO Throw if node is not of type plane
		if (!std::dynamic_pointer_cast<geometry::Plane>(m_context.selectedNode))
		{
			throw std::runtime_error("Node is not of type Plane");
		}

		MaterialModifier materialManager{m_context, m_commandManager};
		return materialManager;
	}

} // namespace st::application