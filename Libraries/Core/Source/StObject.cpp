#include "StObject.hpp"
#include "StObjectPrivate.hpp"

namespace st::core
{
    /*---------------------*/
    /*-------Private-------*/
    /*---------------------*/
    StObject::PrivateStObject::PrivateStObject():
        m_type(StObject::Type::eUnknown)
    {

    }


    /*---------------------*/
    /*-------Public--------*/
    /*---------------------*/
    StObject::StObject(): m_privateObject(std::make_unique<PrivateStObject>())
    {
    }

    StObject::~StObject() = default;

    StObject::StObject(StObject&&) = default;

    StObject& StObject::operator=(StObject&&) = default;

    void StObject::setType(Type type)
    {
        m_privateObject->m_type = type;
    }

    StObject::Type StObject::getType() const
    {
        return m_privateObject->m_type;
    }

    bool StObject::isOfType(Type type) const
    {
        return m_privateObject->m_type == type;
    }


} // namespace st::core