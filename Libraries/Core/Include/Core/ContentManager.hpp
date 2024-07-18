#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "EventSystem.hpp"
#include "Nodes/NodeGraph.hpp"
#include "Nodes/Node.hpp"
#include "Handlers.hpp"
#include <memory>
#include <print>
#include <vector>

#include <spdlog/spdlog.h>


namespace st::core
{

	class ContentManager
	{
	  public:
		ContentManager(EventSystemHandler eventSystem) :
			m_eventSystem(eventSystem),
			m_nodeGraph(eventSystem)
		{
			spdlog::info("ContentManager::ContentManager()");
		}

		void initialize()
		{
			// m_nodeGraph.initialize();
			spdlog::info("ContentManager::initialize()");

			//At this time all nodes that are registered with the content manager should be initialized
			spdlog::info("ContentManager::initialize() - Number of registered nodes: {}", NodeRegistry::getInstance().getNodeInitializers().size());


			spdlog::info("ContentManager::initialize() - Done");
		}

		void run()
		{
			spdlog::info("ContentManager::run()");
			m_nodeGraph.evaluate();
		}

		NodeGraphHandler getMainNodeGraph()
		{
			return NodeGraphHandler(&m_nodeGraph);
		}

	  private:
		EventSystemHandler m_eventSystem;
		NodeGraph m_nodeGraph;
		//NodeRegistry m_nodeRegistry;
	};


	using ContentManagerHandler = Handler<ContentManager>;

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP