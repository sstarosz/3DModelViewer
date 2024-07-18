#ifndef ST_CORE_HANDLERS_HPP
#define ST_CORE_HANDLERS_HPP

#include <memory>

namespace st::core
{


template <typename Component>
class Handler
{
    public:
        Handler()
            : m_pComponent(nullptr)
        {
        }

        Handler(Component* component) :
            m_pComponent(component)
        {
        }

        Component* operator->()
        {
            assert(m_pComponent != nullptr);
            return m_pComponent;
        }

    private:    
        Component* m_pComponent;
};






} // namespace st::core

#endif // ST_CORE_HANDLERS_HPP