#ifndef ST_CORE_NOTE_HPP
#define ST_CORE_NOTE_HPP


#include "Node.hpp"

#include <list>
#include <vector>
#include <memory>
#include <algorithm>
#include <print>

namespace st::core
{
    class EventSystem;


  	class NodeGraph
	{
	  public:
		NodeGraph(EventSystem& eventSystem);

		void addNode(std::shared_ptr<Node> node);

		void addConnection(std::shared_ptr<Node> sourceNode,
						   std::shared_ptr<Attribute> sourceAttrName,
						   std::shared_ptr<Node> targetNode,
						   std::shared_ptr<Attribute> targetAttrName);

		void evaluate();

		std::vector<std::shared_ptr<Node>> getNodes() const;
		std::vector<std::shared_ptr<Connection>> getConnections() const;

	  private:
		EventSystem& m_eventSystem;

		std::vector<std::shared_ptr<Node>> m_nodes;
		std::vector<std::shared_ptr<Connection>> m_connections;
	};


    class NodeGraphHandler
    {
    public:
        NodeGraphHandler()
            : m_pNodeGraph(nullptr)
        {
        }

        NodeGraphHandler(NodeGraph* nodeGraph) :
            m_pNodeGraph(nodeGraph)
        {
        }

        void addNode(std::shared_ptr<Node> node)
        {
            assert(m_pNodeGraph != nullptr);
            m_pNodeGraph->addNode(node);
        }

        void addConnection(std::shared_ptr<Node> sourceNode,
                           std::shared_ptr<Attribute> sourceAttrName,
                           std::shared_ptr<Node> targetNode,
                           std::shared_ptr<Attribute> targetAttrName)
        {
            assert(m_pNodeGraph != nullptr);
            m_pNodeGraph->addConnection(sourceNode, sourceAttrName, targetNode, targetAttrName);
        }

        void evaluate()
        {
            assert(m_pNodeGraph != nullptr);
            m_pNodeGraph->evaluate();
        }

        std::vector<std::shared_ptr<Node>> getNodes() const
        {
            assert(m_pNodeGraph != nullptr);
            return m_pNodeGraph->getNodes();
        }

        std::vector<std::shared_ptr<Connection>> getConnections() const
        {
            assert(m_pNodeGraph != nullptr);
            return m_pNodeGraph->getConnections();
        }



    private:
        NodeGraph* m_pNodeGraph;
    };

} // namespace st::core

#endif // ST_CORE_NOTE_HPP