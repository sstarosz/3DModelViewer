#include "Grid.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::geometry::test
{

    class GridTest : public ::testing::Test
    {

    };


    TEST_F(GridTest, GridConstructor)
    {
        Grid grid(10.0F, 1);

        //EXPECT_EQ(grid.getLines().size(), 20);

        //        x  y   z
        // P1 = (-5, 0, -5)
        // P2 = (-5, 0,  5)
        // P3 = ( 5, 0, -5)
        // P4 = ( 5, 0,  5)

        //Test Vertices
        ASSERT_THAT(grid.getVertices().size(), testing::Eq(4));
        ASSERT_THAT(grid.getVertices().at(0), testing::Eq(Vertex(Eigen::Vector3f{-5.0F, 0.0F, -5.0F})));
        ASSERT_THAT(grid.getVertices().at(1), testing::Eq(Vertex(Eigen::Vector3f{-5.0F, 0.0F, 5.0F})));
        ASSERT_THAT(grid.getVertices().at(2), testing::Eq(Vertex(Eigen::Vector3f{5.0F, 0.0F, -5.0F})));
        ASSERT_THAT(grid.getVertices().at(3), testing::Eq(Vertex(Eigen::Vector3f{5.0F, 0.0F, 5.0F})));
   
        //Test Indices
        ASSERT_THAT(grid.getIndices().size(), testing::Eq(8));
        ASSERT_THAT(grid.getIndices(), testing::ElementsAre(0, 2, 0, 1, 1, 3, 2, 3));
    }


} // namespace st::geometry::test