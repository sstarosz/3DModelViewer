#include <Eigen/Core>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <format>
#include <print>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include "Core/Nodes/Node.hpp"

using namespace st;


template <typename T>
void AssertEqual(const T& expected, const T& actual, const std::string& message = "")
{
	if (expected != actual)
	{
		std::cout << "Test failed: " << message << std::endl;
	}
}

// Overload for floating-point comparison with an epsilon value for tolerance
void AssertEqual(float expected, float actual, const std::string& message = "", float epsilon = 0.0001f)
{
	if (std::fabs(expected - actual) > epsilon)
	{
		std::cout << "Test failed (float): " << message << std::endl;
	}
	else
	{
		std::cout << "Test passed (float): " << std::endl;
	}
}

void createAttribute()
{
	//auto test = core::AttributeBuilder("Test")
	//				.setReadable(true)
	//				.setWritable(true)
	//				.build();

	auto numericAttribute = core::NumericAttributeBuilder<float>("Test")
								.setReadable(true)
								.setWritable(true)
								.setDefaultValue(0.0f)
								.setLimits(-100.0f, 100.0f)
								.setSoftLimits(-1.0f, 1.0f)
								.build();

	AssertEqual(0.0F, numericAttribute, "Default value is not correct");
	
	numericAttribute += 10.0f;
	AssertEqual(10.0f, numericAttribute, "Value is not correct after addition");

	numericAttribute -= 5.0f;
	AssertEqual(5.0f, numericAttribute, "Value is not correct after subtraction");

	numericAttribute *= 2.0f;
	AssertEqual(10.0f, numericAttribute, "Value is not correct after multiplication");

	numericAttribute /= 2.0f;
	AssertEqual(5.0f, numericAttribute, "Value is not correct after division");

}



int main()
{
	createAttribute();

	return 0;
}