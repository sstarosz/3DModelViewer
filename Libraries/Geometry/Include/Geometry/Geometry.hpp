#ifndef ST_GEOMETRY_GEOMETRY_HPP
#define ST_GEOMETRY_GEOMETRY_HPP

#include "Atribute.hpp"
#include <map>

namespace st::geometry
{

	/**
	 * @brief
	 *
	 */
	class Geometry
	{
	  public:
		template <typename DataType>
		void addAttribute(const std::string& name, const Attribute::Type& type, const DataType& value)
		{
			attributes.insert(std::make_pair(Attribute(name, type), AttributeValue(value)));
		}

	  private:
		std::map<Attribute, AttributeValue> attributes;
	};

} // namespace st::geometry

#endif //  ST_GEOMETRY_GEOMETRY_HPP