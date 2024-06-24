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
        m_nodeGraph.addNode(node.lock());
    }

    NodeGraph& getMainNodeGraph()
    {
        return m_nodeGraph;
    }

    private:
    core::NodeGraph m_nodeGraph;
};

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP