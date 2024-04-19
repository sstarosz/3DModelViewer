#include "Geometry.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::geometry::test
{

    class GeometryTest : public ::testing::Test
    {

    };

    class GeometryMock : public Geometry
    {
    public:
        MOCK_METHOD(const std::vector<Vertex>, getVertices, (), (const));
        MOCK_METHOD(const std::vector<uint32_t>, getIndices, (), (const));
    };

    TEST_F(GeometryTest, GridConstructor)
    {
        GeometryMock geometry;
        ASSERT_TRUE(true);
    }


} // namespace st::geometry::test