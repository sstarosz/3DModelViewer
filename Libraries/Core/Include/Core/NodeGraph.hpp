#ifndef ST_CORE_NOTE_HPP
#define ST_CORE_NOTE_HPP

#include <list>
#include <vector>
#include <memory>
#include <algorithm>
#include <print>

namespace st::core
{


    /**
     * @brief Node graph is a owner of all nodes in the scene
     * 
     */


    class Node
    {
     public:

        enum class Type
        {
            eUnknown,
            eMesh,
            eCamera,
            eLight,
            eTransform,
            eMaterial,
            eScene,
            eTypeCount
        };

        static std::string toString(Type type)
        {
            switch (type)
            {
            case Type::eUnknown:
                return "Unknown";
            case Type::eMesh:
                return "Mesh";
            case Type::eCamera:
                return "Camera";
            case Type::eLight:
                return "Light";
            case Type::eTransform:
                return "Transform";
            case Type::eMaterial:
                return "Material";
            case Type::eScene:
                return "Scene";
            default:
                return "Unknown";
            }
        }

        Node() = default;
        Node(Type type) : m_type(type) {}


        bool isOfType(Type type)
        {
            return m_type == type;
        }

        void removeChild(Node* node)
        {
            std::erase(m_children, node);
        }

        void removeParent(Node* node)
        {
            std::erase(m_parents, node);
        }




        Type m_type = Type::eUnknown;
        std::vector<Node*> m_children;
        std::vector<Node*> m_parents;

        //Inputs to the node
        

        //Outputs from the node

    };

	class NodeGraph
	{
	  public:
		Node* addNode(Node&& node)
		{
			auto& newNode = m_nodes.emplace_back(std::make_unique<Node>(node));
			return newNode.get();
		}

        void removeNode(Node* node)
        {
            for (Node* child : node->m_children)
            {
                child->removeParent(node);
            }
            for (Node* parent : node->m_parents)
            {
                parent->removeChild(node);
            }

            std::erase_if(m_nodes, [node](const std::unique_ptr<Node>& n) { return n.get() == node; });

        }

        //Debug
        void dumpNodeGraph() const
        {
            for (const auto& node : m_nodes)
            {
                std::print("Node: {}\n", Node::toString(node->m_type));
            }
        }

	  public:
		std::vector<std::unique_ptr<Node>> m_nodes;
	};

} // namespace st::core

#endif // ST_CORE_NOTE_HPP