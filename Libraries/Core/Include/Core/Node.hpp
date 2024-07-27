#ifndef ST_CORE_NODES_NODE_HPP
#define ST_CORE_NODES_NODE_HPP

#include "Attribute.hpp"
#include "NumericAttribute.hpp"
#include "TypedAttribute.hpp"

#include <any>
#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <print>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <spdlog/spdlog.h>

namespace st::core
{
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

		Node();
		Node(const std::string& name);

		virtual ~Node() = default;

		/*----------------------*/
		/*-------Getters--------*/
		/*----------------------*/

		void defineNode(const std::string& name);

		void addAttribute(std::shared_ptr<Attribute> attribute);

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

		NodeState getState() const;

		bool isDirty() const;
		bool isUninitialized() const;

	  private:
		std::string m_name;
		std::vector<std::shared_ptr<Attribute>> m_attributes;
		NodeState m_state;
	};

} // namespace st::core

#endif // ST_CORE_NODES_NODE_HPP