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
								.build();


}



int main()
{
	

	return 0;
}