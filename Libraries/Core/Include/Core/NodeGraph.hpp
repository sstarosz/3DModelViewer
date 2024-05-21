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


    class NodeDefinition
    {

    };

    class Node
    {
     public:
        virtual ~Node() = default;
    };

	class NodeGraph : public Node
	{

	};



} // namespace st::core

#endif // ST_CORE_NOTE_HPP