#include "RendererCommands.hpp"

#include "Renderer/Nodes/StandardMaterial.hpp"
#include "Core/Nodes/CameraNode.hpp"

namespace st::renderer
{

	CreateRendererCommand::CreateRendererCommand(core::ContentManagerHandler contentManager, std::shared_ptr<core::Node> camera) :
		m_contentManager(contentManager),
		m_camera(camera)
	{
	}

	std::shared_ptr<renderer::Renderer> CreateRendererCommand::execute()
	{
		m_renderer = std::make_shared<renderer::Renderer>();
		m_renderer->initialize();

		core::NodeGraph& nodeGraph = m_contentManager->getMainNodeGraph();
		nodeGraph.addNode(m_renderer);

		// Make material nodes automatically connect to renderer

		//Find the Renderable attribute in the renderer
		std::shared_ptr<core::Attribute> targetAttribute = nullptr;
		for (auto& attribute : m_renderer->getAttributes())
		{
			if (std::shared_ptr<core::TypedAttribute<renderer::Renderable>> renderable = std::dynamic_pointer_cast<core::TypedAttribute<renderer::Renderable>>(attribute))
			{
				std::println("Found Renderable");
				targetAttribute = attribute;
			}
		}

		// Find all material nodes and connect them to the renderer
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


		//Connect the camera to the renderer
		std::shared_ptr<core::Attribute> cameraInputAttribute = nullptr;
		for (auto& attribute : m_renderer->getAttributes())
		{
			if (std::shared_ptr<core::TypedAttribute<core::Camera>> cameraInput = std::dynamic_pointer_cast<core::TypedAttribute<core::Camera>>(attribute))
			{
				std::println("Found Camera Input");
				cameraInputAttribute = attribute;
			}
		}



		if (m_camera)
		{
			for (auto& attribute : m_camera->getAttributes())
			{
				if (std::shared_ptr<core::TypedAttribute<core::Camera>> cameraOutputAttribute = std::dynamic_pointer_cast<core::TypedAttribute<core::Camera>>(attribute))
				{
					std::println("Found Camera Output");
					nodeGraph.addConnection(m_camera, cameraOutputAttribute, m_renderer, cameraInputAttribute);
				}
			}
		}

		return m_renderer;
	}

	void CreateRendererCommand::undo()
	{
		// TODO implement
		// m_contentManager.remove(m
	}
	
} // namespace st::renderer