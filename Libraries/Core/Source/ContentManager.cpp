#include "ContentManager.hpp"
#include <spdlog/spdlog.h>

namespace st::core
{
	ContentManager::ContentManager() :
		m_nodeGraph()
	{
	}

	bool ContentManager::onStart()
	{
		spdlog::info("ContentManager::onStart()");

		m_nodeGraph.evaluate();

		return true;
	}
	
	NodeGraph& ContentManager::getMainNodeGraph()
	{
		return m_nodeGraph;
	}

	const NodeGraph& ContentManager::getMainNodeGraph() const
	{
		return m_nodeGraph;
	}

	void ContentManager::updateSelection(std::weak_ptr<Node> node)
	{
		spdlog::info("ContentManager::updateSelection()");

		if (auto nodePtr = node.lock())
		{
			m_selection.m_paths.clear();
			m_selection.m_paths.push_back(nodePtr->getPath());

			EventRegistry::sendEvent(core::CoreEvents::SelectionChanged);
		}
	}

	Selection ContentManager::getSelection() const
	{
		return m_selection;
	}

} // namespace st::core