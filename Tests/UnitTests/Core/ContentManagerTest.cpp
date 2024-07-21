#include "ContentManager.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::core::test
{

	class ContentManagerTest : public ::testing::Test
	{

	};

	TEST_F(ContentManagerTest, ContentManagerConstructor)
	{
		ContentManager contentManager;
		ASSERT_TRUE(true);
	}

	TEST_F(ContentManagerTest, ContentManagerInitialize)
	{
		ContentManager contentManager;
		ASSERT_TRUE(contentManager.initialize());
	}

    TEST_F(ContentManagerTest, ContentManagerOnStart)
    {
        ContentManager contentManager;
        ASSERT_TRUE(contentManager.onStart());
    }

} // namespace st::core::test