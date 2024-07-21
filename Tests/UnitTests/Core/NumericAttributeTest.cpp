#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "NumericAttribute.hpp"

namespace st::core::test
{
    
    class NumericAttributesTest : public ::testing::Test
    {

    };

	/*------------------------------------------------*/
	/*-----------------NumericAttribute----------------*/
	/*------------------------------------------------*/

	TEST_F(NumericAttributesTest, NumericAttributeConstructor)
	{
		auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
		ASSERT_TRUE(true);
	}

	TEST_F(NumericAttributesTest, NumericAttributeInitialization)
	{
		auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
		ASSERT_EQ(10.0f, numericAttribute->getData());
	}

	TEST_F(NumericAttributesTest, NumericAttributeSetDefaultValue)
	{
		auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
		numericAttribute->setDefaultValue(0.0f);
		ASSERT_EQ(0.0f, numericAttribute->getDefaultValue());
	}

	TEST_F(NumericAttributesTest, NumericAttributeSetLimits)
	{
		auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
		numericAttribute->setLimits(-100.0f, 100.0f);
		ASSERT_EQ(-100.0f, numericAttribute->getLowerLimit());
		ASSERT_EQ(100.0f, numericAttribute->getUpperLimit());
	}

	TEST_F(NumericAttributesTest, NumericAttributeSetSoftLimits)
	{
		auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
		numericAttribute->setSoftLimits(-1.0f, 1.0f);
		ASSERT_EQ(-1.0f, numericAttribute->getSoftLowerLimit());
		ASSERT_EQ(1.0f, numericAttribute->getSoftUpperLimit());
	}

} // namespace st::core::test