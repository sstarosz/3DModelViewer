#ifndef ST_CORE_ST_OBJECT_PRIVATE_HPP
#define ST_CORE_ST_OBJECT_PRIVATE_HPP

#include "StObject.hpp"

namespace st::core
{
    class StObject::PrivateStObject
    {
    public:
        PrivateStObject();

        StObject::Type m_type;
    };

} // namespace st::core

#endif // ST_CORE_ST_OBJECT_PRIVATE_HPP

