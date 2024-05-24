#ifndef ST_CORE_NODES_NODE_HPP
#define ST_CORE_NODES_NODE_HPP

#include <string>


namespace st::core
{
    class Node2
    {
    public:
        virtual ~Node2() = default;

        void defineNode(const std::string& name)
        {
            m_name = name;
        }


        std::string getName() const
        {
            return m_name;
        }

        private:
        std::string m_name;
    };

} // namespace st::core

#endif // ST_CORE_NODES_NODE_HPP