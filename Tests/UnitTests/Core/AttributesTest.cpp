#include "Attribute.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::core::test
{

	class AttributeTest : public ::testing::Test
	{
	};

	class TestAttribute : public Attribute
	{
	  public:
		class Builder : public Attribute::Builder<Builder>
		{
		  public:
			Builder(const std::string& name) :
				Attribute::Builder<Builder>(name)
			{
			}

			std::shared_ptr<TestAttribute> build()
			{
				auto attribute = std::shared_ptr<TestAttribute>(new TestAttribute());
                attribute->setName(this->m_name);
                attribute->setReadable(this->m_readable);
                attribute->setWritable(this->m_writable);
				return attribute;
			}
		};

		friend class Builder;
		virtual ~TestAttribute() = default;

	  protected:
		TestAttribute() = default;
	};

	TEST_F(AttributeTest, AttributeConstructor)
	{
		auto attribute = TestAttribute::Builder("TestAttribute").build();
		ASSERT_TRUE(true);
	}

    TEST_F(AttributeTest, AttributeGetName)
    {
        auto attribute = TestAttribute::Builder("TestAttribute").build();
        ASSERT_EQ("TestAttribute", attribute->getName());
    }


	TEST_F(AttributeTest, AttributeSetReadable)
	{
		auto attribute = TestAttribute::Builder("TestAttribute").setReadable(true).build();
		ASSERT_TRUE(attribute->isReadable());
	}

	TEST_F(AttributeTest, AttributeSetWritable)
	{
		auto attribute = TestAttribute::Builder("TestAttribute").setWritable(true).build();
        ASSERT_TRUE(attribute->isWritable());
	}

} // namespace st::core::test