#include "RendererCommands.hpp"

#include "Renderer/Nodes/StandardMaterial.hpp"

namespace st::renderer
{

	CreateRendererCommand::CreateRendererCommand(core::ContentManagerHandler contentManager) :
		m_contentManager(contentManager)
	{
	}

	void CreateRendererCommand::execute()
	{
		m_renderer = std::make_shared<renderer::Renderer>();
		m_renderer->initialize();

		core::NodeGraph& nodeGraph = m_contentManager->getMainNodeGraph();
		nodeGraph.addNode(m_renderer);

		// Make material nodes automatically connect to renderer

		std::shared_ptr<core::Attribute> targetAttribute = nullptr;
		for (auto& attribute : m_renderer->getAttributes())
		{
			if (std::shared_ptr<core::TypedAttribute<renderer::Renderable>> renderable = std::dynamic_pointer_cast<core::TypedAttribute<renderer::Renderable>>(attribute))
			{
				std::println("Found Renderable");
				targetAttribute = attribute;
			}
		}

		for (auto& node : nodeGraph.getNodes())
		{
			if (std::dynamic_pointer_cast<renderer::StandardMaterial>(node))
			{
				for (auto& attribute : node->getAttributes())
				{
					if (std::shared_ptr<core::TypedAttribute<renderer::Renderable>> renderable = std::dynamic_pointer_cast<core::TypedAttribute<renderer::Renderable>>(attribute))
					{
						std::println("Found Renderable");
						nodeGraph.addConnection(node, renderable, m_renderer, targetAttribute);
					}
				}
			}
		}
	}

	void CreateRendererCommand::undo()
	{
		// TODO implement
		// m_contentManager.remove(m
	}

	std::shared_ptr<renderer::Renderer> CreateRendererCommand::getResult()
	{
		return m_renderer;
	}

} // namespace st::renderer