#include "NodeGraph.hpp"

#include <ranges>
#include <algorithm>

namespace st::core
{
	NodeGraph::NodeGraph()
	{
	}

    Path calculatePath(std::weak_ptr<Node> node)
    {
        
        std::vector<std::string> segments;
        std::shared_ptr<Node> currentNode = node.lock();
        while (currentNode != nullptr)
        {
            segments.push_back(currentNode->getName());
            currentNode = currentNode->getParentNode().lock();
        }

        std::string path = "/World";
        for(const auto& part : std::views::reverse(segments))
        {
            path += "/" + part;
        }


        return Path(path);
    }

	void NodeGraph::addNode(std::shared_ptr<Node> node)
	{
        //Calculate all the paths for the node and its attributes
        node->setPath(calculatePath(node));
        for (const auto& attribute : node->getAttributes())
        {
            attribute->setPath(node->getPath().getPath() + "." + attribute->getName());
        }

        //Generate a unique handle for the node
        node->setHandle(NodeHandle{m_nodes.size() + 1});
        node->addNodeGraph(this);

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
        spdlog::info("NodeGraph::addConnection() - Connection added from {} to {}", sourceNode->getName(), targetNode->getName());
        targetAttrName->setData(sourceAttrName);
    }

    void NodeGraph::initialize()
    {
        for(auto& node : m_nodes)
        {
            node->initialize();
        }
    }

    void NodeGraph::evaluate()
    {
        for(auto& node : m_nodes)
        {
            if(node->isDirty())
            {
                node->compute();
                node->markClean();
            }
        }
    }

    void NodeGraph::evaluate2()
    {
        auto executionOrder = buildExecutionOrder();

        for(auto& node : executionOrder)
        {
            if(node->isDirty())
            {
                node->compute();

                // Mark all connected nodes as dirty
                for (const auto& connection : m_connections)
                {
                    if (connection->sourceNode == node)
                    {
                        connection->targetNode->markDirty();
                    }
                }

                // Mark the node as clean
                node->markClean();
            }
        }
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