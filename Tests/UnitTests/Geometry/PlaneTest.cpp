#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Geometry/Nodes/Plane.hpp"


namespace st::geometry::test
{

class PlaneTest : public ::testing::Test
{
};

TEST_F(PlaneTest, MeshConstructor)
{
    Plane plane;
    ASSERT_TRUE(true);
}

TEST_F(PlaneTest, PlaneInitialize)
{
    Plane plane;
    ASSERT_TRUE(plane.initialize());
}

TEST_F(PlaneTest, PlaneCompute)
{
    Plane plane;
    ASSERT_TRUE(plane.initialize());
    ASSERT_TRUE(plane.compute());
}

TEST_F(PlaneTest, PlaneSubdivide1x1)
{
    /*
    vexpos:
    {
        vtx[0] = {-0.5,  0.0,  0.5}
        vtx[1] = { 0.5,  0.0,  0.5}
        vtx[2] = {-0.5,  0.0, -0.5}
        vtx[3] = { 0.5,  0.0, -0.5}
    }

    indices:
    {
        0, 1, 2,
        1, 3, 2
    }
    
    */
    Plane plane;
    plane.initialize(); 

    //Set subdivisions with to 1
    //Set subdivisions height to 1
    //Set width to 1
    //Set height to 1
    plane.setHeight(1.0F);
    plane.setWidth(1.0F);
    plane.setSubdivisionWidth(1);
    plane.setSubdivisionHeight(1);

    //Check output
    plane.compute();

    //Check if the number of vertices is correct
    ASSERT_EQ(plane.getMesh().getVertexPointList().size(), 4);
    ASSERT_EQ(plane.getMesh().getIndicesPointList().size(), 6);

    //Check if the vertices are correct
    Eigen::Vector3f expectedVertex0{-0.5, -0.5, 0.0};
    Eigen::Vector3f expectedVertex1{ 0.5, -0.5, 0.0};
    Eigen::Vector3f expectedVertex2{-0.5,  0.5, 0.0};
    Eigen::Vector3f expectedVertex3{ 0.5,  0.5, 0.0};

    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(0).isApprox(expectedVertex0));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(1).isApprox(expectedVertex1));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(2).isApprox(expectedVertex2));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(3).isApprox(expectedVertex3));

}


TEST_F(PlaneTest, PlaneSubdivide2x2)
{
    Plane plane;
    plane.initialize();

    //Set subdivisions with to 2
    //Set subdivisions height to 2
    //Set width to 1
    //Set height to 1

    plane.setHeight(1.0F);
    plane.setWidth(1.0F);

    plane.setSubdivisionWidth(2);
    plane.setSubdivisionHeight(2);

    //Check output
    ASSERT_TRUE(plane.compute());

    //Check if the number of vertices is correct
    ASSERT_EQ(plane.getMesh().getVertexPointList().size(), 9);
    ASSERT_EQ(plane.getMesh().getIndicesPointList().size(), 24);

    /*
    vexpos:
    {
        vtx[0] = {-0.5, 0.0, 0.5}
        vtx[1] = {0.0, 0.0, 0.5}
        vtx[2] = {0.5, 0.0, 0.5}
        vtx[3] = {-0.5, 0.0, 0.0}
        vtx[4] = {0.0, 0.0, 0.0}
        vtx[5] = {0.5, 0.0, 0.0}
        vtx[6] = {-0.5, 0.0, -0.5}
        vtx[7] = {0.0, 0.0, -0.5}
        vtx[8] = {0.5, 0.0, -0.5}
    }
    */

    //Check if the vertices are correct
    Eigen::Vector3f expectedVertex0{-0.5, -0.5, 0.0};
    Eigen::Vector3f expectedVertex1{ 0.0, -0.5, 0.0};
    Eigen::Vector3f expectedVertex2{ 0.5, -0.5, 0.0};
    Eigen::Vector3f expectedVertex3{-0.5,  0.0, 0.0};
    Eigen::Vector3f expectedVertex4{ 0.0,  0.0, 0.0};
    Eigen::Vector3f expectedVertex5{ 0.5,  0.0, 0.0};
    Eigen::Vector3f expectedVertex6{-0.5,  0.5, 0.0};
    Eigen::Vector3f expectedVertex7{ 0.0,  0.5, 0.0};
    Eigen::Vector3f expectedVertex8{ 0.5,  0.5, 0.0};

    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(0).isApprox(expectedVertex0));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(1).isApprox(expectedVertex1));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(2).isApprox(expectedVertex2));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(3).isApprox(expectedVertex3));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(4).isApprox(expectedVertex4));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(5).isApprox(expectedVertex5));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(6).isApprox(expectedVertex6));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(7).isApprox(expectedVertex7));
    ASSERT_TRUE(plane.getMesh().getVertexPointList().at(8).isApprox(expectedVertex8));

}



} // namespace st::geometry::test