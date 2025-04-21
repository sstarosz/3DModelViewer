#ifndef ST_CORE_NOTE_HPP
#define ST_CORE_NOTE_HPP


#include "Core/Handlers.hpp"
#include "Node.hpp"
#include "Path.hpp"

#include <list>
#include <vector>
#include <memory>
#include <algorithm>
#include <ranges>
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

		std::weak_ptr<Node> getNodeByPath(const Path& path) const
		{
			using namespace std::string_view_literals;


			if(path.isNode())
			{
				//for example: /World/nodeA/nodeB/nodeC
				// skip the first part of the path
				// and get the node name

				auto paths = path.m_path | std::views::split("/"sv);
				auto it = paths.begin();
				it = std::ranges::next(it, 2); // skip the first two parts (/ and World)
				std::string_view nodeName{(*it).begin(), (*it).end()};

				for (auto& node : m_nodes)
				{
					if (node->getName() == nodeName)
					{
						return node;
					}
				}
			}

			return std::weak_ptr<Node>();
		}

		std::weak_ptr<Attribute> getAttributeByPath(const Path& path) const
		{
			using namespace std::string_view_literals;

			if(path.isAttribute())
			{
				auto paths = path.m_path | std::views::split("/"sv);
				// Get the last part
				std::string_view lastPart;
				for (const auto& part : paths)
				{
					lastPart = std::string_view(part.begin(), part.end());
				}

				// Get the node name
				auto nodePath = path.getPath().substr(0, path.getPath().find_last_of('.'));
				auto node = getNodeByPath(nodePath);
				if (node.expired())
				{
					return std::weak_ptr<Attribute>();
				}

				// Get the attribute name
				std::string_view attributeName = lastPart.substr(lastPart.find('.') + 1, lastPart.length() - lastPart.find('.'));
				for (const auto& attribute : node.lock()->getAttributes())
				{
					if (attribute->getName() == attributeName)
					{
						return attribute;
					}
				}

				// If the attribute is not found, return an empty weak pointer
				return std::weak_ptr<Attribute>();
			}

			return std::weak_ptr<Attribute>();
		}

		Path getPathFromNode(std::weak_ptr<Node> node) const
		{
			if (node.expired())
			{
				return Path();
			}

			//Iterate from node to root node
			std::string path = "/World/";
			std::shared_ptr<Node> currentNode = node.lock();
			while (currentNode)
			{
				path += currentNode->getName() + "/";
				currentNode = currentNode->getParentNode().lock();
			}
			path.pop_back(); // remove the last "/"
			return Path(path);
		}

	  private:
		std::vector<std::shared_ptr<Node>> m_nodes;
		std::vector<std::shared_ptr<Connection>> m_connections;
	};


    using NodeGraphHandler = Handler<NodeGraph>;


} // namespace st::core

#endif // ST_CORE_NOTE_HPP