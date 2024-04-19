#ifndef ST_CORE_OBJECT_HPP
#define ST_CORE_OBJECT_HPP

#include <memory>

namespace st::core
{
    class StObject
    {
    public:
        enum class Type
        {
            eMesh,
            eCamera,
            eTypeCount
        };

    public:
        StObject();
        ~StObject();
        StObject(const StObject&) = delete;
        StObject(StObject&&);
        StObject& operator=(const StObject&) = delete;
        StObject& operator=(StObject&&);

        void setType(Type type);
        Type getType() const;
        bool isOfType(Type type) const;

    private:
        class PrivateStObject;
        std::unique_ptr<PrivateStObject> m_privateObject;
    };

} // namespace st::core

#endif // ST_CORE_OBJECT_HPP