#include "Core/Nodes/CameraNode.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <numbers>
namespace st::core::test
{
    void compareMatrices(const Eigen::Matrix4f&  actual, const Eigen::Matrix4f& expected, float epsilon = 0.01f)
    {
        bool match = true;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (std::abs(actual(i, j) - expected(i, j)) > epsilon)
                {
                    match = false;
                    break;
                }
            }
        }

        if (!match)
        {
            std::cout << "Actual: " << std::endl << actual << std::endl;
            std::cout << "Expected: " << std::endl << expected << std::endl;
        }

        EXPECT_TRUE(match);
    }



    
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

    //TEST_F(CameraTest, Compute)
    //{
    //    CameraNode camera;
    //    EXPECT_TRUE(camera.compute());
    //}

    

    /*
    Requirements:
    - The camera is looking at the origin (0, 0, 0)
    - The camera is located at (0, 0, 5)
    - Half the width of the camera is 500.0f which means rotation around the Y axis by 360.
    - Mouse movement by 125.0f pixels in the X axis should rotate the camera around the Y axis by 90 degrees

    Expected:
    - The view matrix should be:
    [1, 0, 0, 0]
    [0, 1, 0, 0]
    [0, 0, 1, -5]
    [0, 0, 0, 1]

    */
    TEST_F(CameraTest, RotateCamera_ByEmulatingMouseMovement_InYAxis_By90Degrees)
    {
        CameraNode camera;
        camera.setWidth(1000.0f);
        camera.setHeight(1000.0f);
        camera.setPosition(Eigen::Vector3f(0.0f, 0.0f, 5.0f));
        camera.setTarget(Eigen::Vector3f(0.0f, 0.0f, 0.0f));




        const float deltax = 125.0f;
        camera.orbit(deltax, 0.0f);

        Eigen::Matrix4f viewMatrix = camera.getViewMatrix();

        // The expected view matrix in row major order
        Eigen::Matrix4f expectedViewMatrix;
        expectedViewMatrix << 0 , 0 , 1 , 0,
                              0 , 1 , 0 , 0,
                              -1, 0 , 0 , -5,
                              0 , 0 , 0 , 1;

        //EXPECT_TRUE(viewMatrix.isApprox(expectedViewMatrix, 0.01f));
        // Check if the view matrix matches the expected view matrix
        compareMatrices(viewMatrix, expectedViewMatrix);
    }

}
