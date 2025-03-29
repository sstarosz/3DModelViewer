#include "Node.hpp"

#include "Eigen/Dense"

namespace st::core
{

	Node::Node() :
		m_name("Node"),
		m_attributes(),
		m_state(NodeState::eUninitialized)
	{
	}

	Node::Node(const std::string& name) :
		m_name(name),
		m_attributes(),
		m_state(NodeState::eUninitialized)
	{
	}

	void Node::defineNode(const std::string& name)
	{
		m_name = name;
	}

	void Node::addAttribute(std::shared_ptr<Attribute> attribute)
	{
		m_attributes.push_back(attribute);
	}


    Eigen::Matrix4f Node::getLocalMatrix() const
	{
		return Eigen::Matrix4f::Identity();
	}

Node::NodeState Node::getState() const
    {
        return m_state;
    }

	void Node::addChildNode(std::shared_ptr<Node> childNode)
	{
		m_childNodes.push_back(childNode);
		childNode->m_parentNode = shared_from_this();
	}

	void Node::removeChildNode(std::shared_ptr<Node> childNode)
	{
		auto it = std::remove(m_childNodes.begin(), m_childNodes.end(), childNode);
		if (it != m_childNodes.end())
		{
			m_childNodes.erase(it, m_childNodes.end());
			childNode->m_parentNode.reset();
		}
	}
	std::vector<std::shared_ptr<Node>> Node::getChildNodes() const
	{
		return m_childNodes;
	}

	std::weak_ptr<Node> Node::getParentNode() const
	{
		return m_parentNode;
	}

	Eigen::Matrix4f Node::getInclusiveMatrix() const
	{
		Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
		std::shared_ptr<Node> currentNode = m_parentNode.lock();
		while (currentNode)
		{
			matrix = currentNode->getLocalMatrix() * matrix;
			currentNode = currentNode->m_parentNode.lock();
		}

		return matrix;
	}





    bool Node::isUninitialized() const
    {
        return m_state == NodeState::eUninitialized;
    }

    bool Node::isDirty() const
    {
        return m_state == NodeState::eDirty;
    }

} // namespace st::core