#include "Core/Path.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <numbers>
namespace st::core::test
{


    
    class PathTest : public ::testing::Test
    {


    };


   TEST_F(PathTest, DefaultConstructorValues)
    {
        Path path;
        EXPECT_EQ(path.getPath(), "");
    }

    TEST_F(PathTest, CheckIfPathIsNode)
    {
        Path path("/World/NodeA");

        EXPECT_EQ(path.getPath(), "/World/NodeA");
        EXPECT_TRUE(path.isNode());

        path = Path("/World/NodeA/NodeB");
        EXPECT_EQ(path.getPath(), "/World/NodeA/NodeB");
        EXPECT_TRUE(path.isNode());

        path = Path("/World/NodeA/NodeB/NodeC");
        EXPECT_EQ(path.getPath(), "/World/NodeA/NodeB/NodeC");
        EXPECT_TRUE(path.isNode());

        path = Path("/World/NodeA/NodeB.AttributeA");
        EXPECT_EQ(path.getPath(), "/World/NodeA/NodeB.AttributeA");
        EXPECT_FALSE(path.isNode());
    }

    TEST_F(PathTest, CheckIfPathIsAttribute)
    {
        Path path("/World/NodeA.AttributeA");

        EXPECT_EQ(path.getPath(), "/World/NodeA.AttributeA");
        EXPECT_TRUE(path.isAttribute());

        path = Path("/World/NodeA/NodeB.AttributeB");
        EXPECT_EQ(path.getPath(), "/World/NodeA/NodeB.AttributeB");
        EXPECT_TRUE(path.isAttribute());

        path = Path("/World/NodeA/NodeB");
        EXPECT_EQ(path.getPath(), "/World/NodeA/NodeB");
        EXPECT_FALSE(path.isAttribute());
    }


}
