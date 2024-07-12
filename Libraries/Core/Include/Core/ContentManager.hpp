#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "EventSystem.hpp"
#include "Nodes/NodeGraph.hpp"
#include "Nodes/Node.hpp"
#include <memory>
#include <print>
#include <vector>

#include <spdlog/spdlog.h>

namespace st::core
{

	class ContentManager
	{
	  public:
		ContentManager(EventSystem& eventSystem) :
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

		void add(std::weak_ptr<core::Node> node)
		{
			m_nodeGraph.addNode(node.lock());
		}

		NodeGraphHandler getMainNodeGraph()
		{
			return NodeGraphHandler(&m_nodeGraph);
		}

	  private:
		EventSystem& m_eventSystem;
		NodeGraph m_nodeGraph;
		//NodeRegistry m_nodeRegistry;
	};


    class ContentManagerHandler
    {
    public:
        ContentManagerHandler()
            : m_pContentManager(nullptr)
        {
        }

        ContentManagerHandler(ContentManager* contentManager) :
            m_pContentManager(contentManager)
        {
        }

        ContentManager* operator->()
		{
			assert(m_pContentManager != nullptr);
			return m_pContentManager;
		}

        private:
            ContentManager* m_pContentManager;
    };

        

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP