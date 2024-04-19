#include "Camera.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::core::test
{
    
    class CameraTest : public ::testing::Test
    {

    };


    TEST_F(CameraTest, CameraConstructor)
    {
        Camera camera;
        ASSERT_TRUE(true);
    }

}