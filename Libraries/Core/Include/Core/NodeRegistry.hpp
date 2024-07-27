#ifndef ST_CORE_NOTEREGISTRY_HPP
#define ST_CORE_NOTEREGISTRY_HPP

#include "Node.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace st::core
{

	template <typename NodeType>
	class NodeFactory
	{
	  public:
		static std::shared_ptr<Node> createNode()
		{
			return std::make_shared<NodeType>();
		}
	};

	template <typename NodeType>
	class NodeInitializer
	{
	  public:
		static bool initializeNode()
		{
			return true;
		}
	};

	class NodeRegistry
	{

		using NodeCreator = std::function<std::shared_ptr<Node>()>;
		using NodeInitializer = std::function<bool()>;

	  public:
		static NodeRegistry& getInstance()
		{
			static NodeRegistry instance;
			return instance;
		}

		void registerNode(const std::string& name, NodeCreator creator, NodeInitializer initializer)
		{
			m_nodes[name] = creator;
			m_nodeInitializers.push_back(initializer);
		}

		std::shared_ptr<Node> createNode(const std::string& name)
		{
			auto it = m_nodes.find(name);
			if (it != m_nodes.end())
			{
				return it->second();
			}
			else
			{
				return nullptr;
			}
		}

		std::vector<NodeInitializer> getNodeInitializers() const
		{
			return m_nodeInitializers;
		}

	  private:
		NodeRegistry() = default;

		std::unordered_map<std::string, NodeCreator> m_nodes;
		std::vector<NodeInitializer> m_nodeInitializers;
	};

	template <typename NodeType>
	class RegisterNode
	{
	  public:
		RegisterNode(const std::string& name)
		{
			static_assert(std::is_base_of<Node, NodeType>::value, "NodeType must derive from Node");

			NodeRegistry::getInstance().registerNode(name, &NodeFactory<NodeType>::createNode, &NodeInitializer<NodeType>::initializeNode);
		}
	};

} // namespace st::core

#endif // ST_CORE_NOTEREGISTRY_HPP