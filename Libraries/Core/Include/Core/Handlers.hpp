#ifndef ST_CORE_HANDLERS_HPP
#define ST_CORE_HANDLERS_HPP

#include <cassert>

namespace st::core
{

	template <typename Component>
	class Handler
	{
	  public:
		Handler() :
			m_pComponent(nullptr)
		{
		}

		Handler(Component* component) :
			m_pComponent(component)
		{
            assert(m_pComponent != nullptr && "Component is nullptr");
		}

		Component* operator->()
		{
			assert(m_pComponent != nullptr && "Component is nullptr");
			return m_pComponent;
		}

	  private:
		Component* m_pComponent;
	};

} // namespace st::core

#endif // ST_CORE_HANDLERS_HPP