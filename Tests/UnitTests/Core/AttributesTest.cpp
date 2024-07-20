#include "Node.hpp" //TODO split this header file into multiple files

#include <gmock/gmock.h>
#include <gtest/gtest.h>



namespace st::core::test
{
    
    class AttributeTest : public ::testing::Test
    {

    };


    TEST_F(AttributeTest, AttributeConstructor)
    {
        //Attribute attribute;
        ASSERT_TRUE(true);
    }

    TEST_F(AttributeTest, AttributeSetReadable)
    {
        //Attribute attribute;
        //attribute.setReadable(true);
        //ASSERT_TRUE(attribute.isReadable());
    }

    TEST_F(AttributeTest, AttributeSetWritable)
    {
        //Attribute attribute;
        //attribute.setWritable(true);
        //ASSERT_TRUE(attribute.isWritable());
    }

    /*------------------------------------------------*/
    /*-----------------NumericAttribute----------------*/
    /*------------------------------------------------*/


    TEST_F(AttributeTest, NumericAttributeConstructor)
    {
        auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
        ASSERT_TRUE(true);
    }

    TEST_F(AttributeTest, NumericAttributeInitialization)
    {
        auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
        ASSERT_EQ(10.0f, numericAttribute->getData());
    }


    TEST_F(AttributeTest, NumericAttributeSetDefaultValue)
    {
        auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
        numericAttribute->setDefaultValue(0.0f);
        ASSERT_EQ(0.0f, numericAttribute->getDefaultValue());
    }

    TEST_F(AttributeTest, NumericAttributeSetLimits)
    {
        auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
        numericAttribute->setLimits(-100.0f, 100.0f);
        ASSERT_EQ(-100.0f, numericAttribute->getLowerLimit());
        ASSERT_EQ(100.0f, numericAttribute->getUpperLimit());
    }

    TEST_F(AttributeTest, NumericAttributeSetSoftLimits)
    {
        auto numericAttribute = NumericAttribute<float>::Builder("TestAttribute", 10.0f).build();
        numericAttribute->setSoftLimits(-1.0f, 1.0f);
        ASSERT_EQ(-1.0f, numericAttribute->getSoftLowerLimit());
        ASSERT_EQ(1.0f, numericAttribute->getSoftUpperLimit());
    }





}