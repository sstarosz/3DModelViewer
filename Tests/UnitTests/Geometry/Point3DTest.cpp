#include "Point3D.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::geometry::test
{

    class Point3DTest : public ::testing::Test
    {

    };

    TEST_F(Point3DTest, GridConstructor)
    {
        Point3D geometry;
        ASSERT_TRUE(true);
    }


} // namespace st::geometry::test