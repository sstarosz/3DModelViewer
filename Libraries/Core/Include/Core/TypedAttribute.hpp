#ifndef ST_CORE_TYPEDATTRIBUTE_HPP
#define ST_CORE_TYPEDATTRIBUTE_HPP

#include "Attribute.hpp"

namespace st::core
{

	template <typename Type>
	class TypedAttribute : public Attribute
	{
	  public:
		class Builder : public Attribute::Builder<Builder>
		{
		  public:
			Builder(const std::string& name) :
				Attribute::Builder<Builder>(name),
				m_defaultValue(Type())
			{
			}

			Builder& setDefaultValue(Type value)
			{
				m_defaultValue = value;
				return *this;
			}

			std::shared_ptr<TypedAttribute> build()
			{
				auto typedAttribute = std::shared_ptr<TypedAttribute>(new TypedAttribute());
				typedAttribute->setName(this->m_name);
				typedAttribute->setReadable(this->m_readable);
				typedAttribute->setWritable(this->m_writable);
				typedAttribute->setData(m_defaultValue);

				return typedAttribute;
			}

		  private:
			Type m_defaultValue;
		};

		friend class Builder;
		~TypedAttribute() = default;

		Type getData() const
		{
			return Attribute::getData<Type>();
		}

		void setData(Type value)
		{
			Attribute::setData<Type>(value);
		}

	  protected:
		TypedAttribute() = default;
	};

	/*----------------------*/
	/*-------Handlers-------*/
	/*----------------------*/

	template <typename Type>
	class TypedInputHandler : public AttributeHandler
	{
	  public:
		TypedInputHandler() = default;

		TypedInputHandler(std::shared_ptr<TypedAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		operator Type () const
		{
			return m_attribute->getValue();
		}

		TypedInputHandler<Type>& operator= (const TypedInputHandler<Type>& rhs)
		{
			m_attribute = rhs.m_attribute;
			return *this;
		}

		TypedInputHandler<Type>& operator= (const Type& rhs)
		{
			m_attribute->setData(rhs);
			return *this;
		}

		Type getData() const
		{
			return m_attribute->getData();
		}

		std::shared_ptr<TypedAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<TypedAttribute<Type>> m_attribute;
	};

	template <typename Type>
	class TypedOutputHandler : public AttributeHandler
	{
	  public:
		TypedOutputHandler() = default;

		TypedOutputHandler(std::shared_ptr<TypedAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		operator Type () const
		{
			return m_attribute->getData();
		}

		TypedOutputHandler<Type>& operator= (const Type& rhs)
		{
			assert(m_attribute && "Attribute is not initialized");
			m_attribute->setData(rhs);
			return *this;
		}

		Type getData() const
		{
			return m_attribute->getData();
		}

		std::shared_ptr<TypedAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<TypedAttribute<Type>> m_attribute;
	};

} // namespace st::core

#endif // ST_CORE_TYPEDATTRIBUTE_HPP