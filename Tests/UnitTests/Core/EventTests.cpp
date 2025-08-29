#include "Core/EventRegistry.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <numbers>
namespace st::core::test
{
    class EventTest : public ::testing::Test
    {


    };

    TEST_F(EventTest, AttributeChangedEvent)
    {

        EventBus::subscribe<AttributeChangedEvent>(
            [](const AttributeChangedEvent& event)
            {
                EXPECT_EQ(event.getMessage(), AttributeMessage::eAttributeChanged);
                EXPECT_EQ(event.getPath(), Path("testPath"));
            });

        EventBus::publish<AttributeChangedEvent>(
            AttributeChangedEvent{AttributeMessage::eAttributeChanged, Path("testPath")});

        // Check if the event was published and received correctly
        
    }
}
