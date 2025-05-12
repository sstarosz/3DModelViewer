#ifndef ST_CORE_EVENTREGISTRY_HPP
#define ST_CORE_EVENTREGISTRY_HPP

#include "Path.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <compare>
#include <functional>


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
    enum class AttributeMessage
    {
        eAttributeChanged
    };

    using AttributeChangedCallback = std::function<void(AttributeMessage, Path)>;


    enum class NodeMessage
    {
        eNodeGraphChanged
    };

    using NodeChangedCallback = std::function<void(NodeMessage, Path)>;




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