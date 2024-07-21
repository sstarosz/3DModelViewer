#include "Node.hpp"

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


    Node::NodeState Node::getState() const
    {
        return m_state;
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