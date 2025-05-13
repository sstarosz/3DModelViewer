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
#include <unordered_set>

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

		void initialize();
		void evaluate();
		void evaluate2();

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

		std::weak_ptr<Node> getNodeByAttributePath(const Path& path) const;

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

		 void propagateDirty(std::shared_ptr<Node> node);
	 	 void propagateDirtyRecursive(std::shared_ptr<Node> node, std::unordered_set<std::shared_ptr<Node>>& visitedNodes);
	
		void disableEvaluation()
		{
			m_isEvaluationDisabled = true;
		}

		void enableEvaluation()
		{
			m_isEvaluationDisabled = false;
		}

	private:
		bool m_isEvaluating{false};
		bool m_isDirtyPropagating{false};
		bool m_isEvaluationDisabled{false};
		std::vector<std::shared_ptr<Node>> m_nodes;
		std::vector<std::shared_ptr<Connection>> m_connections;
		std::vector<std::shared_ptr<Attribute>> m_attributes;

		std::vector<std::shared_ptr<Node>> buildExecutionOrder()
		{
			std::vector<std::shared_ptr<Node>> executionOrder;
			
			std::unordered_set<std::shared_ptr<Node>> visitedNodes;
			std::unordered_set<std::shared_ptr<Node>> inProcess;

			for(const auto& node : m_nodes)
			{
				if (visitedNodes.find(node) == visitedNodes.end())
				{
					topologicalSort(node, visitedNodes, inProcess, executionOrder);
				}
			}

			return executionOrder;
		}

		void topologicalSort(std::shared_ptr<Node> node,
							 std::unordered_set<std::shared_ptr<Node>>& visitedNodes,
							 std::unordered_set<std::shared_ptr<Node>>& inProcess,
							 std::vector<std::shared_ptr<Node>>& executionOrder)
		{
			// Check if the node is in process (to detect cycles)
			if (inProcess.find(node) != inProcess.end())
			{
				throw std::runtime_error("Cyclic dependency detected in node graph.");
			}

			// Check if the node is already visited or in process
			if (visitedNodes.find(node) != visitedNodes.end())
			{
				return;
			}

			inProcess.insert(node);

			for (const auto& childNode : node->getChildNodes())
			{
				topologicalSort(childNode, visitedNodes, inProcess, executionOrder);
			}

			inProcess.erase(node);
			visitedNodes.insert(node);
			executionOrder.push_back(node);
		}
	};


    using NodeGraphHandler = Handler<NodeGraph>;


} // namespace st::core

#endif // ST_CORE_NOTE_HPP