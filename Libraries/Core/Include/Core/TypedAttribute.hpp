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

} // namespace st::core

#endif // ST_CORE_TYPEDATTRIBUTE_HPP