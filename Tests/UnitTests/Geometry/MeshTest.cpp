#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Mesh.hpp"


namespace st::geometry::test
{

class MeshTest : public ::testing::Test
{
};

TEST_F(MeshTest, MeshConstructor)
{
    Mesh mesh;
    ASSERT_TRUE(true);
}


} // namespace st::geometry::test