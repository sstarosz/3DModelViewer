#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "EventSystem.hpp"
#include "Nodes/NodeGraph.hpp"
#include "Nodes/Node.hpp"
#include <memory>
#include <print>
#include <vector>

namespace st::core
{

	class ContentManager
	{
	  public:
		ContentManager(EventSystem& eventSystem) :
			m_eventSystem(eventSystem),
			m_nodeGraph(eventSystem)
		{
		}

		void initialize()
		{
			// m_nodeGraph.initialize();
			std::println("ContentManager::initialize()");
			std::println("Size of m_nodeGraph.getNodes(): {}", m_nodeGraph.getNodes().size());

			for (auto& node : m_nodeGraph.getNodes())
			{
				node->initialize();
			}
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