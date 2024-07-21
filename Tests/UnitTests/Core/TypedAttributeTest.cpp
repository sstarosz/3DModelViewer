#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "TypedAttribute.hpp"

#include <compare>

namespace st::core::test
{
    
    class TypedAttributesTest : public ::testing::Test
    {

    };

	struct MyType
	{

		auto operator <=>(const MyType&) const = default;
	};


	TEST_F(TypedAttributesTest, TypedAttributeConstructor)
	{
		auto typedAttribute = TypedAttribute<MyType>::Builder("TestAttribute").build();
		ASSERT_TRUE(true);
	}

	TEST_F(TypedAttributesTest, TypedAttributeGetName)
	{
		auto typedAttribute = TypedAttribute<MyType>::Builder("TestAttribute").build();
		ASSERT_EQ("TestAttribute", typedAttribute->getName());
	}
	
	TEST_F(TypedAttributesTest, TypedAttributeGetData)
	{
		auto typedAttribute = TypedAttribute<MyType>::Builder("TestAttribute").build();
		MyType data;
		ASSERT_THAT(typedAttribute->getData(), testing::Eq(data));
	}


	/*----------------------------*/
	/*-------Input Handlers-------*/
	/*----------------------------*/
	TEST_F(TypedAttributesTest, TypedAttributeInputHandler)
	{
		core::TypedInputHandler<MyType> handler = TypedAttribute<MyType>::Builder("TestAttribute").build();
		ASSERT_TRUE(true);
	}

	TEST_F(TypedAttributesTest, TypedAttributeInputHandlerGetData)
	{
		core::TypedInputHandler<MyType> handler = TypedAttribute<MyType>::Builder("TestAttribute").build();
		MyType data;
		ASSERT_THAT(handler.getData(), testing::Eq(data));
	}

	
	/*-----------------------------*/
	/*-------Output Handlers-------*/
	/*-----------------------------*/

	TEST_F(TypedAttributesTest, TypedAttributeOutputHandler)
	{
		core::TypedOutputHandler<MyType> handler = TypedAttribute<MyType>::Builder("TestAttribute").build();
		ASSERT_TRUE(true);
	}

} // namespace st::core::test