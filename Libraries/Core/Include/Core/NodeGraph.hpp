#ifndef ST_CORE_NOTE_HPP
#define ST_CORE_NOTE_HPP


#include "Core/Handlers.hpp"
#include "Node.hpp"

#include <list>
#include <vector>
#include <memory>
#include <algorithm>
#include <print>

namespace st::core
{

	class Connection
	{
	  public:
		Connection(std::shared_ptr<Node> sourceNode,
				   std::shared_ptr<Attribute> sourceAttrName,
				   std::shared_ptr<Node> targetNode,
				   std::shared_ptr<Attribute> targetAttrName) :
			sourceNode(sourceNode),
			sourceAttrName(sourceAttrName),
			targetNode(targetNode),
			targetAttrName(targetAttrName)
		{
		}

		std::shared_ptr<Node> sourceNode;
		std::shared_ptr<Attribute> sourceAttrName;

		std::shared_ptr<Node> targetNode;
		std::shared_ptr<Attribute> targetAttrName;
	};


  	class NodeGraph
	{
	  public:
		NodeGraph();

		void addNode(std::shared_ptr<Node> node);

		void addConnection(std::shared_ptr<Node> sourceNode,
						   std::shared_ptr<Attribute> sourceAttrName,
						   std::shared_ptr<Node> targetNode,
						   std::shared_ptr<Attribute> targetAttrName);

		void evaluate();

		std::vector<std::shared_ptr<Node>> getNodes() const;
		std::vector<std::shared_ptr<Connection>> getConnections() const;

	  private:
		std::vector<std::shared_ptr<Node>> m_nodes;
		std::vector<std::shared_ptr<Connection>> m_connections;
	};


    using NodeGraphHandler = Handler<NodeGraph>;


} // namespace st::core

#endif // ST_CORE_NOTE_HPP