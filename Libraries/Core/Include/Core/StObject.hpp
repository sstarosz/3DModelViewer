#ifndef ST_CORE_OBJECT_HPP
#define ST_CORE_OBJECT_HPP

#include <memory>

namespace st::core
{
    class StObject
    {
    public:
        StObject();
        ~StObject();
        StObject(const StObject&) = delete;
        StObject(StObject&&) = delete;
        StObject& operator=(const StObject&) = delete;
        StObject& operator=(StObject&&) = delete;


    private:
        class PrivateStObject;
        std::unique_ptr<PrivateStObject> m_privateObject;
    };

} // namespace st::core

#endif // ST_CORE_OBJECT_HPP