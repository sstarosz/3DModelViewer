#ifndef ST_EVENT_SYSTEM_HPP
#define ST_EVENT_SYSTEM_HPP

#include "StObject.hpp"

#include <memory>
#include <unordered_map>

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
        std::weak_ptr<StObject> m_eventData;
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
        static EventSystem& getInstance()
        {
            static EventSystem instance;
            return instance;
        }

        template<typename Listener>
        static void registerEvent(Event::Type eventType, Listener listener)
        {
            getInstance().registerEventPrivate(eventType, listener);
        }


        //Send event to all listeners
        static void sendEvent(Event event)
        {
            getInstance().sendEventPrivate(event);
        }


    private:
        EventSystem()
        {

        }
        

        template<typename Listener>
        void registerEventPrivate(Event::Type eventType, Listener listener)
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

} // namespace st::core

#endif // ST_CORE_SCENE_HPP
