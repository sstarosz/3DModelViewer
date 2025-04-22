#include "ContentManager.hpp"
#include <spdlog/spdlog.h>

namespace st::core
{
	ContentManager::ContentManager() :
		m_nodeGraph()
	{
		spdlog::info("ContentManager::ContentManager()");

		spdlog::info("ContentManager::ContentManager() - Done");
	}

	bool ContentManager::initialize()
	{
		spdlog::info("ContentManager::initialize()");

		spdlog::info("ContentManager::initialize() - Done");

		return true;
	}

	bool ContentManager::onStart()
	{
		spdlog::info("ContentManager::onStart()");

		//m_nodeGraph.initialize();
		m_nodeGraph.evaluate();

		spdlog::info("ContentManager::onStart() - Done");

		return true;
	}
	
	NodeGraph& ContentManager::getMainNodeGraph()
	{
		return m_nodeGraph;
	}

	const NodeGraph&  ContentManager::getMainNodeGraph() const
	{
		return m_nodeGraph;
	}

	void ContentManager::updateSelection(std::weak_ptr<Node> node)
	{
		spdlog::info("ContentManager::updateSelection()");

		if (node.expired())
		{
			spdlog::warn("ContentManager::updateSelection() - Node is expired");
			return;
		}

		m_selectedNode = node;
		m_selection.m_paths.clear();
		m_selection.m_paths.push_back(m_nodeGraph.getPathFromNode(node));


		EventRegistry::sendEvent(core::CoreEvents::SelectionChanged);
		spdlog::info("ContentManager::updateSelection() - Done");
	}

	std::weak_ptr<Node> ContentManager::getSelectedNode() const
	{
		return m_selectedNode;
	}

} // namespace st::core