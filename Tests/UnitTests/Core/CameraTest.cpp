#include "Core/Nodes/CameraNode.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::core::test
{
    
    class CameraTest : public ::testing::Test
    {

    };


   TEST_F(CameraTest, DefaultConstructorValues)
    {
        CameraNode camera;
        EXPECT_EQ(camera.getAngleOfView(), 0.0f);
        EXPECT_EQ(camera.getFocalLength(), 0.0f);
        EXPECT_EQ(camera.getNearClippingPlane(), 0.0f);
        EXPECT_EQ(camera.getFarClippingPlane(), 0.0f);
    }

    TEST_F(CameraTest, ConstructorValues)
    {
        CameraNode camera(45.0f, 50.0f, 0.1f, 100.0f);
        EXPECT_EQ(camera.getAngleOfView(), 45.0f);
        EXPECT_EQ(camera.getFocalLength(), 50.0f);
        EXPECT_EQ(camera.getNearClippingPlane(), 0.1f);
        EXPECT_EQ(camera.getFarClippingPlane(), 100.0f);
    }

    TEST_F(CameraTest, Setters)
    {
        CameraNode camera;
        camera.setAngleOfView(45.0f);
        camera.setFocalLength(50.0f);
        camera.setNearClippingPlane(0.1f);
        camera.setFarClippingPlane(100.0f);
        EXPECT_EQ(camera.getAngleOfView(), 45.0f);
        EXPECT_EQ(camera.getFocalLength(), 50.0f);
        EXPECT_EQ(camera.getNearClippingPlane(), 0.1f);
        EXPECT_EQ(camera.getFarClippingPlane(), 100.0f);
    }

    TEST_F(CameraTest, Initialize)
    {
        CameraNode camera;
        EXPECT_TRUE(camera.initialize());
    }

    TEST_F(CameraTest, Compute)
    {
        CameraNode camera;
        EXPECT_TRUE(camera.compute());
    }

    
}