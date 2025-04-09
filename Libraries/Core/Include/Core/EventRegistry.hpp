#ifndef ST_CORE_EVENTREGISTRY_HPP
#define ST_CORE_EVENTREGISTRY_HPP

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

      private:
        std::unordered_map<EventId, std::string> m_eventMap;
        std::unordered_map<EventId, std::vector<EventCallback>> m_eventCallbacks;
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