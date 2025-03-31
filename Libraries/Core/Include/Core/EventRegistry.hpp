#ifndef ST_CORE_EVENTREGISTRY_HPP
#define ST_CORE_EVENTREGISTRY_HPP

#include <cstdint>

namespace st::core
{
    class EventId
    {
    public:
        uint32_t id = 0;
    };



    class EventRegistry
    {
      public:
        EventRegistry() = default;
        ~EventRegistry() = default;

        // Add event handling methods here

        constexpr static EventRegistry& instance()
        {
            static EventRegistry instance;
            return instance;
        }

        EventId registerEvent(const char* eventName)
        {
            // Register the event and return its ID
            EventId eventId;
            eventId.id = ++m_lastEventId;
            return eventId;
        }

      private:
        uint32_t m_lastEventId = 0;
    };


} // namespace st::core

#endif // ST_CORE_EVENTREGISTRY_HPP