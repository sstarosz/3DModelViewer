#ifndef ST_CORE_NUMERICATTRIBUTE_HPP
#define ST_CORE_NUMERICATTRIBUTE_HPP

#include "Attribute.hpp"

namespace st::core
{

	template <typename Type>
	concept NumericAttributeType = std::is_same<Type, bool>::value ||
								   std::is_same<Type, int32_t>::value ||
								   std::is_same<Type, uint32_t>::value ||
								   std::is_same<Type, float>::value ||
								   std::is_same<Type, double>::value;

	template <NumericAttributeType Type>
	class NumericAttribute : public Attribute
	{
	  public:
		class Builder : public Attribute::Builder<Builder>
		{
		  public:
			Builder(const std::string& name, Type defaultValue) :
				Attribute::Builder<Builder>(name),
				m_defaultValue(defaultValue),
				m_min(defaultValue),
				m_max(defaultValue),
				m_softMin(defaultValue),
				m_softMax(defaultValue)
			{
			}

			Builder& setDefaultValue(Type value)
			{
				m_defaultValue = value;
				return *this;
			}

			Builder& setLimits(Type min, Type max)
			{
				m_min = min;
				m_max = max;
				return *this;
			}

			Builder& setSoftLimits(Type min, Type max)
			{
				m_softMin = min;
				m_softMax = max;
				return *this;
			}

			std::shared_ptr<NumericAttribute> build()
			{
				auto numericAttribute = std::shared_ptr<NumericAttribute>(new NumericAttribute());
				numericAttribute->setName(this->m_name);
				numericAttribute->setReadable(this->m_readable);
				numericAttribute->setWritable(this->m_writable);
				numericAttribute->setData(m_defaultValue);
				numericAttribute->setDefaultValue(m_defaultValue);
				numericAttribute->setLimits(m_min, m_max);
				numericAttribute->setSoftLimits(m_softMin, m_softMax);

				return numericAttribute;
			}

		  private:
			Type m_defaultValue;
			Type m_min;
			Type m_max;
			Type m_softMin;
			Type m_softMax;
		};

		friend class Builder;
		~NumericAttribute() = default;

	  protected:
		NumericAttribute() = default;

	  public:
		Type getData() const
		{
			return Attribute::getData<Type>();
		}

		void setData(Type value)
		{
			Attribute::setData<Type>(value);
		}

		/*----------------------*/
		/*-------Setters--------*/
		/*----------------------*/

		void setDefaultValue(Type value)
		{
			// TODO - check for limits
			m_properties.defaultValue = value;
		}

		void setLimits(Type min, Type max)
		{
			m_properties.min = min;
			m_properties.max = max;
		}

		void setSoftLimits(Type min, Type max)
		{
			m_properties.softMin = min;
			m_properties.softMax = max;
		}

		/*----------------------*/
		/*-------Getters--------*/
		/*----------------------*/
		Type getDefaultValue() const
		{
			return m_properties.defaultValue;
		}

		Type getLowerLimit() const
		{
			return m_properties.min;
		}

		Type getUpperLimit() const
		{
			return m_properties.max;
		}

		Type getSoftLowerLimit() const
		{
			return m_properties.softMin;
		}

		Type getSoftUpperLimit() const
		{
			return m_properties.softMax;
		}

	  private:
		struct NumericAttributeProperties
		{
			Type defaultValue;
			Type min;
			Type max;
			Type softMin;
			Type softMax;
		};

		NumericAttributeProperties m_properties;
	};


	/*----------------------*/
	/*-------Handlers-------*/
	/*----------------------*/
    /**
	 * @brief NumericInputHandler is a class that provide handler over numeric attributes
	 * for easy access and manipulation on generic attributes
	 *
	 * Inputs are always readable and writable
	 */
	template <NumericAttributeType Type> // TODO add Constraint
	class NumericInputHandler : public AttributeHandler
	{
	  public:
		NumericInputHandler() = default;

		NumericInputHandler(std::shared_ptr<NumericAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		operator Type () const
		{
			return m_attribute->getData();
		}

		NumericInputHandler<Type>& operator= (const Type& rhs)
		{
			m_attribute->setData(rhs);
			return *this;
		}

		NumericInputHandler& operator+= (const Type& rhs)
		{
			Type value = m_attribute->getData();
			value += rhs;
			return *this;
		}

		NumericInputHandler& operator-= (const Type& rhs)
		{
			Type value = m_attribute->getData();
			value -= rhs;
			return *this;
		}

		NumericInputHandler& operator*= (const Type& rhs)
		{
			Type value = m_attribute->getData();
			value *= rhs;
			return *this;
		}

		NumericInputHandler& operator/= (const Type& rhs)
		{
			Type value = m_attribute->getData();
			value /= rhs;
			return *this;
		}

		std::shared_ptr<NumericAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<NumericAttribute<Type>> m_attribute;
	};

	template <NumericAttributeType Type>
	class NumericOutputHandler : public AttributeHandler
	{
	  public:
		NumericOutputHandler() = default;

		NumericOutputHandler(std::shared_ptr<NumericAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

        operator Type () const
        {
            return m_attribute->getData();
        }

		Type getData() const
		{
			return m_attribute->getData();
		}

		std::shared_ptr<NumericAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<NumericAttribute<Type>> m_attribute;
	};

} // namespace st::core

#endif // ST_CORE_NUMERIC_ATTRIBUTE_HPP