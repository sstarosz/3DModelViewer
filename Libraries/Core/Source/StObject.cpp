#include "StObject.hpp"


namespace st::core
{
    class StObject::PrivateStObject
    {
    public:
        PrivateStObject()
        {
        }

        private:
        int m_dummy;
    };

    /*---------------------*/
    /*-------Public--------*/
    /*---------------------*/
    StObject::StObject(): m_privateObject(std::make_unique<PrivateStObject>())
    {
    }

    StObject::~StObject() = default;


} // namespace st::core