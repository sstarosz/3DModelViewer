#ifndef ST_CORE_CORE_HPP
#define ST_CORE_CORE_HPP

#include <cstdint>

namespace st::core
{

    // Forward declarations
    class Node;
    class Attribute;
    class Path;

    // Type aliases
    using NodeHandle = std::uint64_t;
    static constexpr NodeHandle InvalidNodeHandle{0};

    using AttributeHandle = std::uint64_t;
    static constexpr AttributeHandle InvalidAttributeHandle{0};


} // namespace st::core

#endif // ST_CORE_CORE_HPP