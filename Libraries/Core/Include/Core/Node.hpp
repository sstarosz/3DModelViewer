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
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/Core>

namespace st::core
{
	/*----------------------*/
	/*-------Node----------*/
	/*----------------------*/
	class Node : public std::enable_shared_from_this<Node>
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

		template <typename Type>
		void addAttribute(NumericInputHandler<Type> attribute)
		{
			addAttribute(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(NumericOutputHandler<Type> attribute)
		{
			addAttribute(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(TypedInputHandler<Type> attribute)
		{
			addAttribute(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(TypedOutputHandler<Type> attribute)
		{
			addAttribute(attribute.getAttribute());
		}

		void addAttribute(std::shared_ptr<Attribute> attribute)
		{
			m_attributes.push_back(attribute);
		}


		virtual bool initialize() = 0;
		virtual bool compute() = 0;
		virtual Eigen::Matrix4f getLocalMatrix() const; 

		std::string getName() const
		{
			return m_name;
		}

		std::vector<std::shared_ptr<Attribute>> getAttributes() const
		{
			return m_attributes;
		}

		NodeState getState() const;

		void addChildNode(std::shared_ptr<Node> childNode);
		void removeChildNode(std::shared_ptr<Node> childNode);
		std::vector<std::shared_ptr<Node>> getChildNodes() const;
		std::weak_ptr<Node> getParentNode() const;
		Eigen::Matrix4f getInclusiveMatrix() const;



		bool isDirty() const;
		bool isUninitialized() const;

		void setPath(const Path& path)
		{
			m_path = path;
		}
		
		Path getPath() const
		{
			return m_path;
		}

	  private:
		std::string m_name;
		std::vector<std::shared_ptr<Attribute>> m_attributes;
		NodeState m_state;
		std::weak_ptr<Node> m_parentNode;
		std::vector<std::shared_ptr<Node>> m_childNodes;
		Path m_path;
	};

} // namespace st::core

#endif // ST_CORE_NODES_NODE_HPP