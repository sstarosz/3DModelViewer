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

		m_nodeGraph.evaluate();

		spdlog::info("ContentManager::onStart() - Done");

		return true;
	}

	NodeGraphHandler ContentManager::getMainNodeGraph()
	{
		return NodeGraphHandler(&m_nodeGraph);
	}

} // namespace st::core