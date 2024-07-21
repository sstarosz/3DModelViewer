#ifndef ST_CORE_NODES_NODE_HPP
#define ST_CORE_NODES_NODE_HPP

#include "Attribute.hpp"
#include "NumericAttribute.hpp"
#include "TypedAttribute.hpp"

#include <any>
#include <cassert>
#include <memory>
#include <optional>
#include <print>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <functional>

#include <spdlog/spdlog.h>

namespace st::core
{

	/*----------------------*/
	/*-------Handlers-------*/
	/*----------------------*/
	class AttributeHandler
	{
	};

	/**
	 * @brief NumericInputHandler is a class that provide handler over numeric attributes
	 * for easy access and manipulation on generic attributes
	 *
	 * Inputs are always readable and writable
	 */
	template <NumericAttributeType Type> // TODO add Constraint
	class NumericInputHandler : public AttributeHandler
	{
	  public:
		NumericInputHandler() = default;

		NumericInputHandler(std::shared_ptr<NumericAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		operator Type () const
		{
			return m_attribute->getData();
		}

		NumericInputHandler<Type>& operator= (const Type& rhs)
		{
			m_attribute->setValue(rhs);
			return *this;
		}

		NumericInputHandler& operator+= (const Type& rhs)
		{
			Type value = m_attribute->getData();
			value += rhs;
			return *this;
		}

		NumericInputHandler& operator-= (const Type& rhs)
		{
			Type value = m_attribute->getData();
			value -= rhs;
			return *this;
		}

		NumericInputHandler& operator*= (const Type& rhs)
		{
			Type value = m_attribute->getData();
			value *= rhs;
			return *this;
		}

		NumericInputHandler& operator/= (const Type& rhs)
		{
			Type value = m_attribute->getData();
			value /= rhs;
			return *this;
		}

		std::shared_ptr<NumericAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<NumericAttribute<Type>> m_attribute;
	};

	template <NumericAttributeType Type>
	class NumericOutputHandler : public AttributeHandler
	{
	  public:
		NumericOutputHandler() = default;

		NumericOutputHandler(std::shared_ptr<NumericAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		Type getValue() const
		{
			return m_attribute->getValue();
		}

		std::shared_ptr<NumericAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<NumericAttribute<Type>> m_attribute;
	};

	template <typename Type>
	class TypedInputHandler : public AttributeHandler
	{
	  public:
		TypedInputHandler() = default;

		TypedInputHandler(std::shared_ptr<TypedAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		operator Type () const
		{
			return m_attribute->getValue();
		}

		TypedInputHandler<Type>& operator= (const TypedInputHandler<Type>& rhs)
		{
			m_attribute = rhs.m_attribute;
			return *this;
		}

		TypedInputHandler<Type>& operator= (const Type& rhs)
		{
			m_attribute->setData(rhs);
			return *this;
		}

		Type getValue() const
		{
			return m_attribute->getData();
		}

		std::shared_ptr<TypedAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<TypedAttribute<Type>> m_attribute;
	};

	template <typename Type>
	class TypedOutputHandler : public AttributeHandler
	{
	  public:
		TypedOutputHandler() = default;

		TypedOutputHandler(std::shared_ptr<TypedAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		operator Type () const
		{
			return m_attribute->getValue();
		}

		TypedOutputHandler<Type>& operator= (const Type& rhs)
		{
			assert(m_attribute && "Attribute is not initialized");
			m_attribute->setData(rhs);
			return *this;
		}

		Type getValue() const
		{
			return m_attribute->getValue();
		}

		std::shared_ptr<TypedAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<TypedAttribute<Type>> m_attribute;
	};

	/*----------------------*/
	/*-------Node----------*/
	/*----------------------*/
	class Node
	{
	  public:
		enum class NodeState
		{
			eUninitialized,
			eInitialized,
			eDirty,
			eClean

		};

		Node():
			m_name(""),
			m_state(NodeState::eUninitialized)
		{
		}

		virtual ~Node() = default;

		void defineNode(const std::string& name)
		{
			m_name = name;
		}

		void addAttribute(std::shared_ptr<Attribute> attribute)
		{
			m_attributes.push_back(attribute);
		}

		template <typename Type>
		void addAttribute(NumericInputHandler<Type> attribute)
		{
			m_attributes.push_back(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(NumericOutputHandler<Type> attribute)
		{
			m_attributes.push_back(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(TypedInputHandler<Type> attribute)
		{
			m_attributes.push_back(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(TypedOutputHandler<Type> attribute)
		{
			m_attributes.push_back(attribute.getAttribute());
		}

		virtual bool initialize() = 0;
		virtual bool compute() = 0;


		std::string getName() const
		{
			return m_name;
		}

		std::vector<std::shared_ptr<Attribute>> getAttributes() const
		{
			return m_attributes;
		}

		NodeState getState() const
		{
			return m_state;
		}

		bool isDirty() const
		{
			return m_state == NodeState::eDirty;
		}

		bool isUninitialized() const
		{
			return m_state == NodeState::eUninitialized;
		}

	  private:
		std::string m_name;
		std::vector<std::shared_ptr<Attribute>> m_attributes;
		NodeState m_state;
	};

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

#endif // ST_CORE_NODES_NODE_HPP