#include "StObject.hpp"


namespace st::core
{
    class StObject::PrivateStObject
    {
    public:
        PrivateStObject()
        {
        }

        int32_t m_dummy;
    };

    /*---------------------*/
    /*-------Public--------*/
    /*---------------------*/
    StObject::StObject(): m_privateObject(std::make_unique<PrivateStObject>())
    {
    }

    StObject::~StObject() = default;

    StObject::StObject(StObject&&) = default;

    StObject& StObject::operator=(StObject&&) = default;


} // namespace st::core