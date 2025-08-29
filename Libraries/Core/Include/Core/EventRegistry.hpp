#ifndef ST_CORE_EVENTREGISTRY_HPP
#define ST_CORE_EVENTREGISTRY_HPP

#include "Path.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <compare>
#include <functional>
#include <typeindex>

namespace st::core
{
    class EventId
    {
    public:
        size_t id = 0;

        auto operator<=>(const EventId& other) const = default;
    };
}// namespace st::core

template <>
struct std::hash<st::core::EventId>
{
    size_t operator()(const st::core::EventId& eventId) const noexcept
    {
        return std::hash<size_t>()(eventId.id);
    }
};


namespace st::core
{
    struct Event
    {
    public:
        Event() = default;
        Event(const Event&) = default;
        Event(Event&&) = default;

        Event& operator=(const Event&) = default;
        Event& operator=(Event&&) = default;

        ~Event() = default;
    };


    /*----------------------*/
    /*-------Attribute-----*/
    /*----------------------*/
    enum class AttributeMessage
    {
        eAttributeChanged
    };

    class AttributeChangedEvent : public Event
    {
    public:
        AttributeChangedEvent(AttributeMessage msg, Path path) : m_message(msg), m_path(path) {}

        AttributeMessage getMessage() const
        {
            return m_message;
        }
        
        Path getPath() const
        {
            return m_path;
        }

    private:
        AttributeMessage m_message;
        Path m_path; 
    };

    using AttributeChangedCallback = std::function<void(AttributeMessage, Path)>;


    /*----------------------*/
    /*-------NodeGraph-----*/
    /*----------------------*/
    enum class NodeMessage
    {
        eNodeGraphChanged
    };
 
    struct NodeGraphChangedEvent : public Event
    {
        NodeMessage message;
        Path path;
    };

    using NodeChangedCallback = std::function<void(NodeMessage, Path)>;



    class EventBus
    {
    public:
        template <typename EventType>
        static void subscribe(std::function<void(const EventType&)> callback)
        {
            instance().subscribeInternal<EventType>(callback);
        }

        template <typename EventType>
        static void publish(const EventType& event)
        {
            instance().publishInternal(event);
        }

    private:
        // Private implementation details
        EventBus() = default;
        
        static EventBus& instance()
        {
            static EventBus instance;
            return instance;
        }

        // Internal implementation methods
        template <typename EventType>
        void subscribeInternal(std::function<void(const EventType&)> callback)
        {
            auto& callbacks = m_callbacks[typeid(EventType)];
            callbacks.push_back([callback](const Event& event) {
                callback(static_cast<const EventType&>(event));
            });
        }

        template <typename EventType>
        void publishInternal(const EventType& event)
        {
            auto it = m_callbacks.find(typeid(EventType));
            if (it != m_callbacks.end())
            {
                for (const auto& callback : it->second)
                {
                    callback(event);
                }
            }
        }

        // Type-erased storage for callbacks
        using TypeErasedCallback = std::function<void(const Event&)>;
        std::unordered_map<std::type_index, std::vector<TypeErasedCallback>> m_callbacks;
    };



    class EventRegistry
    {
      public:
        using EventCallback = std::function<void()>;


        EventRegistry() = default;
        ~EventRegistry() = default;

        // Add event handling methods here

        static EventRegistry& instance()
        {
            static EventRegistry instance;
            return instance;
        }

        static EventId registerEvent(const std::string& eventName)
        {
            EventId eventId = EventRegistry::instance().registerEventPrivate(eventName);
            return eventId;
        }

        EventId registerEventPrivate(const std::string& eventName)
        {
            std::hash<std::string> hasher;
            EventId eventId{hasher(eventName)};
            // Register the event and return its ID
            m_eventMap[eventId] = eventName;
            return eventId;
        }

        static void sendEvent(const core::EventId& eventId)
        {
            EventRegistry::instance().sendEventPrivate(eventId);
        } 

        void sendEventPrivate(const core::EventId& eventId)
        {
            auto it = m_eventCallbacks.find(eventId);
            if (it != m_eventCallbacks.end())
            {
                for (const auto& callback : it->second)
                {
                    callback();
                }
            }
        }


        static void subscribeToEvent(const core::EventId& eventId, const EventCallback& callback)
        {
            EventRegistry::instance().subscribeToEventPrivate(eventId, callback);
        }

        void subscribeToEventPrivate(const core::EventId& eventId, const EventCallback& callback)
        {
            m_eventCallbacks[eventId].push_back(callback);
        }

        /*----------------------*/
        /*-------Attribute-----*/
        /*----------------------*/
        static void addAttributeChangedCallback(Path attributePath, const AttributeChangedCallback& callback)
        {
            EventRegistry::instance().addAttributeChangedCallbackPrivate(attributePath, callback);
        }

        void addAttributeChangedCallbackPrivate(Path attributePath, const AttributeChangedCallback& callback)
        {
            m_attributeCallbacks[attributePath].push_back(callback);
        }

        static void sendAttributeChangedEvent(Path attributePath, AttributeMessage msg)
        {
            EventRegistry::instance().sendAttributeChangedEventPrivate(attributePath, msg);
        }

        void sendAttributeChangedEventPrivate(Path attributePath, AttributeMessage msg)
        {
            auto it = m_attributeCallbacks.find(attributePath);
            if (it != m_attributeCallbacks.end())
            {
                for (const auto& callback : it->second)
                {
                    callback(msg, attributePath);
                }
            }
        }


        /*----------------------*/
        /*-------NodeGraph-----*/
        /*----------------------*/

        static void addNodeChangedCallback(Path nodePath, const NodeChangedCallback& callback)
        {
            EventRegistry::instance().addNodeChangedCallbackPrivate(nodePath, callback);
        }

        void addNodeChangedCallbackPrivate(Path nodePath, const NodeChangedCallback& callback)
        {
            m_nodeGraphCallbacks[nodePath].push_back(callback);
        }

        static void sendNodeGraphChangedEvent(Path nodePath, NodeMessage msg)
        {
            EventRegistry::instance().sendNodeGraphChangedEventPrivate(nodePath, msg);
        }

        void sendNodeGraphChangedEventPrivate(Path nodePath, NodeMessage msg)
        {
            auto it = m_nodeGraphCallbacks.find(nodePath);
            if (it != m_nodeGraphCallbacks.end())
            {
                for (const auto& callback : it->second)
                {
                    callback(msg, nodePath);
                }
            }
        }


      private:
        std::unordered_map<EventId, std::string> m_eventMap;
        std::unordered_map<EventId, std::vector<EventCallback>> m_eventCallbacks;
        std::unordered_map<Path, std::vector<AttributeChangedCallback>> m_attributeCallbacks;
        std::unordered_map<Path, std::vector<NodeChangedCallback>> m_nodeGraphCallbacks;
    };

    class CoreEvents
    {
    public:
        inline static EventId SelectionChanged = EventRegistry::registerEvent("SelectionChanged");
        inline static EventId NodeGraphChanged = EventRegistry::registerEvent("NodeGraphChanged");
        inline static EventId NodeGraphLoaded  = EventRegistry::registerEvent("NodeGraphLoaded");
        inline static EventId NodeGraphSaved   = EventRegistry::registerEvent("NodeGraphSaved");
        inline static EventId NodeGraphClosed  = EventRegistry::registerEvent("NodeGraphClosed");
    };

} // namespace st::core





#endif // ST_CORE_EVENTREGISTRY_HPP