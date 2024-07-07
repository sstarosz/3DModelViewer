#include "NodeGraph.hpp"

#include "EventSystem.hpp"

namespace st::core
{
	NodeGraph::NodeGraph(EventSystem& eventSystem) :
		m_eventSystem(eventSystem)
	{
	}



	void NodeGraph::addNode(std::shared_ptr<Node> node)
	{
		m_nodes.push_back(node);
	}

    void NodeGraph::addConnection(std::shared_ptr<Node> sourceNode,
						   std::shared_ptr<Attribute> sourceAttrName,
						   std::shared_ptr<Node> targetNode,
						   std::shared_ptr<Attribute> targetAttrName)
    {
        // TODO

        // Check if sourceAttribute is readable
        // Check if destinationAttribute is writable
        // Check if sourceAttribute and destinationAttribute are compatible (type)
        // Check if sourceAttribute and destinationAttribute are compatible (dimension)
        m_connections.emplace_back(std::make_shared<Connection>(sourceNode, sourceAttrName, targetNode, targetAttrName));
    }



    void NodeGraph::evaluate()
    {
        // TODO
    }

	std::vector<std::shared_ptr<Node>> NodeGraph::getNodes() const
	{
		return m_nodes;
	}

     std::vector<std::shared_ptr<Connection>> NodeGraph::getConnections() const
    {
        return m_connections;
    }

} // namespace st::core