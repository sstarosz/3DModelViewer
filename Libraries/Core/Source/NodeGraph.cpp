#include "NodeGraph.hpp"

#include <ranges>
#include <algorithm>



template<>
struct std::formatter<std::shared_ptr<st::core::Node>>{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const std::shared_ptr<st::core::Node>& node, FormatContext& ctx) const {
        return format_to(ctx.out(), "{}", node->getName());
    }
};

template<>
struct fmt::formatter<std::shared_ptr<st::core::Node>>{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const std::shared_ptr<st::core::Node>& node, FormatContext& ctx) const {
        return format_to(ctx.out(), "{}", node->getName());
    }
};


// Add this formatter for vectors of shared_ptr<Node>
template<>
struct fmt::formatter<std::vector<std::shared_ptr<st::core::Node>>> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const std::vector<std::shared_ptr<st::core::Node>>& nodes, FormatContext& ctx) const {
        // Start with an opening bracket
        fmt::format_to(ctx.out(), "[");
        
        // Format each node with a comma separator
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (i > 0) {
                fmt::format_to(ctx.out(), ", ");
            }
            fmt::format_to(ctx.out(), "{}", nodes[i]->getName());
        }
        
        // End with a closing bracket
        return fmt::format_to(ctx.out(), "]");
    }
};


namespace st::core
{


	NodeGraph::NodeGraph()
	{
        EventRegistry::subscribeToEvent(core::CoreEvents::NodeGraphChanged, [this]()
        {
            spdlog::info("NodeGraph::NodeGraph() - Node graph changed event triggered");
            evaluate2();
        });

	}

    Path calculatePath(std::weak_ptr<Node> node)
    {
        
        std::vector<std::string> segments;
        std::shared_ptr<Node> currentNode = node.lock();
        while (currentNode != nullptr)
        {
            segments.push_back(currentNode->getName());
            currentNode = currentNode->getParentNode().lock();
        }



        std::string path = "/World";
        //Check if the node is a material
        if(auto material = node.lock())
        {
            if(material->getType() == Node::NodeType::eMaterial)
            {
                path = "/Materials";
            }
        }

        for(const auto& part : std::views::reverse(segments))
        {
            path += "/" + part;
        }


        return Path(path);
    }


    std::weak_ptr<Node> NodeGraph::getNodeByAttributePath(const Path& path) const
    {
        if(path.isAttribute())
        {
            auto paths = path.m_path | std::views::split(std::string("/"));
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
                return std::weak_ptr<Node>();
            }

            return node.lock();

            // If the attribute is not found, return an empty weak pointer
            return std::weak_ptr<Node>();
        }

        return std::weak_ptr<Node>();
    }

    void NodeGraph::propagateDirty(std::shared_ptr<Node> node)
    {
        if(m_isDirtyPropagating)
            return;

        m_isDirtyPropagating = true;
        std::unordered_set<std::shared_ptr<Node>> visitedNodes;

        propagateDirtyRecursive(node, visitedNodes);

        m_isDirtyPropagating = false;
    }

    void NodeGraph::propagateDirtyRecursive(std::shared_ptr<Node> node, std::unordered_set<std::shared_ptr<Node>>& visitedNodes)
    {
        // Skip if already visited
        if (visitedNodes.find(node) != visitedNodes.end())
            return;
        
        // Mark as visited
        visitedNodes.insert(node);
        
        // Mark the node as dirty
        node->markDirty();
        
        // Propagate to children
        for (const auto& child : node->getChildNodes()) {
            propagateDirtyRecursive(child, visitedNodes);
        }
        
        // Propagate through connections
        for (const auto& connection : m_connections) {
            if (connection->sourceNode == node) {
                propagateDirtyRecursive(connection->targetNode, visitedNodes);
            }
        }
    }



	void NodeGraph::addNode(std::shared_ptr<Node> node)
	{
        spdlog::info("NodeGraph::addNode() - Adding node: {}", node->getName());
        //Calculate all the paths for the node and its attributes
        node->setPath(calculatePath(node));
        for (const auto& attribute : node->getAttributes())
        {
            attribute->setPath(node->getPath().getPath() + "." + attribute->getName());
        }

        //Generate a unique handle for the node
        node->setHandle(NodeHandle{m_nodes.size() + 1});
        node->addNodeGraph(this);

        //Register node attributes in global registry
        for (const auto& attribute : node->getAttributes())
        {
            spdlog::info("NodeGraph::addNode() - Registering attribute: {} with path: {}", attribute->getName(), attribute->getPath().getPath());

            m_attributes.push_back(attribute);
            EventRegistry::addAttributeChangedCallback(attribute->getPath(), [this, attribute](AttributeMessage msg, Path path)
            {
                if(msg == AttributeMessage::eAttributeChanged)
                {
                    spdlog::info("NodeGraph::addNode() - Attribute changed event triggered for attribute: {}", attribute->getName());

                    std::weak_ptr<Node> node = getNodeByAttributePath(path);

                    if(auto nodePtr = node.lock())
                    {
                        spdlog::info("NodeGraph::addNode() - Attribute changed event triggered for node: {} and attribute: {}", nodePtr->getName(), attribute->getName());
        
                        propagateDirty(nodePtr);

                         // Schedule evaluation if needed
                        EventRegistry::sendEvent(core::CoreEvents::NodeGraphChanged);
                    }
                }
            });  
        }


		m_nodes.push_back(node);
	}

    void NodeGraph::addConnection(std::shared_ptr<Node> sourceNode,
						   std::shared_ptr<Attribute> sourceAttrName,
						   std::shared_ptr<Node> targetNode,
						   std::shared_ptr<Attribute> targetAttrName)
    {
        // TODO

        // Check if sourceAttribute is readable
        // Check if destinationAttribute is writable
        // Check if sourceAttribute and destinationAttribute are compatible (type)
        // Check if sourceAttribute and destinationAttribute are compatible (dimension)
        m_connections.emplace_back(std::make_shared<Connection>(sourceNode, sourceAttrName, targetNode, targetAttrName));
        spdlog::info("NodeGraph::addConnection() - Connection added from {} to {}", sourceNode->getName(), targetNode->getName());
        targetAttrName->setData(sourceAttrName);
    }

    void NodeGraph::initialize()
    {
        for(auto& node : m_nodes)
        {
            node->initialize();
        }
    }

    void NodeGraph::evaluate()
    {
        m_isDirtyPropagating = true;

        for(auto& node : m_nodes)
        {
            if(node->isDirty())
            {
                node->compute();
                node->markClean();
            }
        }

        m_isDirtyPropagating = false;
    }

    void NodeGraph::evaluate2()
    {

        if(m_isEvaluating || m_isEvaluationDisabled)
        {
            spdlog::warn("NodeGraph::evaluate2() - Evaluation is disabled or already in progress");
            return;
        }

        m_isEvaluating = true;

        auto executionOrder = buildExecutionOrder();
        spdlog::warn("NodeGraph::evaluate2() - Execution order: {}", executionOrder.size());
        spdlog::warn("NodeGraph::evaluate2() - Execution order: {}", executionOrder);

        for(auto& node : executionOrder)
        {
            if(node->isDirty())
            {
                node->compute();
                node->markClean();
            }
        }

        m_isEvaluating = false;
    }

	std::vector<std::shared_ptr<Node>> NodeGraph::getNodes() const
	{
		return m_nodes;
	}

    std::vector<std::shared_ptr<Connection>> NodeGraph::getConnections() const
    {
        return m_connections;
    }

} // namespace st::core