#include "Vertex.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::geometry::test
{

    class VertexTest : public ::testing::Test
    {

    };


    TEST_F(VertexTest, VertexConstructor)
    {
        Vertex vertex(Eigen::Vector3f{1.0F, 2.0F, 3.0F});
        ASSERT_THAT(vertex.getPosition(), testing::Eq(Eigen::Vector3f{1.0F, 2.0F, 3.0F}));
    }


} // namespace st::geometry::test