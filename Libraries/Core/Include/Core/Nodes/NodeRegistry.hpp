#ifndef ST_CORE_NOTE_REGISTRY_HPP
#define ST_CORE_NOTE_REGISTRY_HPP

#include "Node.hpp"
#include <unordered_map>
#include <memory>
#include <string>

namespace st::core
{

class NodeRegistry
{
public:
    NodeRegistry() = default;
    ~NodeRegistry() = default;


    void registerNode(std::shared_ptr<Node> node)
    {
        m_nodes.push_back(node);
    }

    std::vector<std::shared_ptr<Node>> getNodes() const
    {
        return m_nodes;
    }


private:
    std::unordered_map<std::string, std::shared_ptr<Node>> m_nodes;
};


} // namespace st::core




#endif // ST_CORE_NOTE_REGISTRY_HPP