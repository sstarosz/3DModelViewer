#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "Nodes/Node.hpp"
#include "NodeGraph.hpp"
#include <memory>
#include <vector>

namespace st::core
{

class ContentManager
{
    public:
    void add(std::weak_ptr<core::Node2> node)
    {
        m_nodes.push_back(node.lock());
    }

    void addToCurrentContext(std::shared_ptr<core::Node> node, std::weak_ptr<core::Node2> context)
    {

    }

    std::vector<std::shared_ptr<core::Node2>> getNodes() const
    {
        return m_nodes;
    }

    private:
    std::vector<std::shared_ptr<core::Node2>> m_nodes;
};

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP