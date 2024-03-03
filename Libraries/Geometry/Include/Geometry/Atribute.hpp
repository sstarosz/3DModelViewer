#ifndef ST_GEOMETRY_ATTRIBUTE_HPP
#define ST_GEOMETRY_ATTRIBUTE_HPP

#include <cstddef>
#include <span>
#include <string>

namespace st::geometry
{
	class Attribute
	{
	  public:
		enum class Type
		{
			// Scalars
			eBoolean,
			eInteger,
			eUnsignedInteger,
			eFloat,
			eDouble,

			// Vectors
			eBooleanVector2,
			eIntegerVector2,
			eUnsignedIntegerVector2,
			eFloatVector2,
			eDoubleVector2,

			eBooleanVector3,
			eIntegerVector3,
			eUnsignedIntegerVector3,
			eFloatVector3,
			eDoubleVector3,

			eBooleanVector4,
			eIntegerVector4,
			eUnsignedIntegerVector4,
			eFloatVector4,
			eDoubleVector4,

			// Matrices
			eBooleanMatrix2x2,
			eIntegerMatrix2x2,
			eUnsignedIntegerMatrix2x2,
			eFloatMatrix2x2,
			eDoubleMatrix2x2,

			eBooleanMatrix3x3,
			eIntegerMatrix3x3,
			eUnsignedIntegerMatrix3x3,
			eFloatMatrix3x3,
			eDoubleMatrix3x3,

			eBooleanMatrix4x4,
			eIntegerMatrix4x4,
			eUnsignedIntegerMatrix4x4,
			eFloatMatrix4x4,
			eDoubleMatrix4x4,

			// Samplers
			eSampler1D,
			eSampler2D,
			eSampler3D,

			// TODO: Add more types

			eMaxAttributeType
		};

		Attribute();
		Attribute(const std::string& name, Type type);

		auto operator<=> (const Attribute&) const = default;

	  private:
		std::string name;
		Type type;
	};

	class AttributeValue
	{
	  public:
		AttributeValue();

		template <typename DataType>
		AttributeValue(const DataType& value)
		{
			// data = std::as_bytes(std::span{value});
		}

	  private:
		std::span<std::byte> data;
	};

} // namespace st::geometry

#endif //  ST_GEOMETRY_ATTRIBUTE_HPP