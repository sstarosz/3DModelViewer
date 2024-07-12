#ifndef ST_EVENT_SYSTEM_HPP
#define ST_EVENT_SYSTEM_HPP

#include "StObject.hpp"

#include <memory>
#include <unordered_map>
#include <functional>

#include <spdlog/spdlog.h>

namespace st::core
{


    class Event
    {
    public:
        enum class Type
        {
            eAddedModel,
        };

        virtual ~Event() = default;

        Type m_eventType;
    };


    class EventData : public Event
    {

    public:
       uint32_t m_eventData;
    };


    class ListenerBase
    {
    public:
        virtual ~ListenerBase() {}
        virtual void call(Event event) const = 0;
    };

    template<typename FunctionType>
    class FunctionWrapperImpl : public ListenerBase
    {
    public:
        FunctionWrapperImpl(FunctionType&& function)
            : m_function(std::forward<FunctionType>(function))
        {
        }

        void call(Event event) const override
        {
            m_function(&event);
        }

    private:
        FunctionType m_function;
    };


    class EventSystem 
    {
    public:
        EventSystem()
        {
			spdlog::info("EventSystem::EventSystem()");
        }

        template<typename Listener>
        void registerEvent(Event::Type eventType, Listener listener)
        {
            m_eventListeners.emplace(eventType, std::make_unique<FunctionWrapperImpl<Listener>>(std::move(listener)));
        }

        void sendEventPrivate(Event event)
        {
            for(auto& listener : m_eventListeners)
            {
                if(listener.first == event.m_eventType)
                {
                    listener.second->call(event);
                }
            }
        }

        std::unordered_map<Event::Type, std::unique_ptr<ListenerBase>> m_eventListeners;
        //class PrivateEventSystem;
        //std::unique_ptr<PrivateEventSystem> m_privateEventSystem;
    };

	class EventSystemHandler
	{
	  public:
		EventSystemHandler() :
			m_eventSystem(nullptr)
		{
		}

		EventSystemHandler(EventSystem* eventSystem) :
			m_eventSystem(eventSystem)
		{
		}

        EventSystem& operator->()
        {
            return *m_eventSystem;
        }

	  private:
		EventSystem* m_eventSystem;
	};

} // namespace st::core

#endif // ST_CORE_SCENE_HPP
