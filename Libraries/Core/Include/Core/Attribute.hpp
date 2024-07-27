#ifndef ST_CORE_ATTRIBUTE_HPP
#define ST_CORE_ATTRIBUTE_HPP

#include <any>
#include <cassert>
#include <memory>
#include <string>

namespace st::core
{

	// TODO Attribute need redesign
	class Attribute
	{
	  public:
		template <typename Derived>
		class Builder
		{
		  public:
			Builder(const std::string& name) :
				m_name(name)
			{
			}

			/*----------------------*/
			/*-------Getters--------*/
			/*----------------------*/
			bool isReadable() const
			{
				return m_readable;
			}

			bool isWritable() const
			{
				return m_writable;
			}

			std::string getName() const
			{
				return m_name;
			}

			/*----------------------*/
			/*-------Setters--------*/
			/*----------------------*/
			Derived& setReadable(bool value)
			{
				m_readable = value;
				return static_cast<Derived&>(*this);
			}

			Derived& setWritable(bool value)
			{
				m_writable = value;
				return static_cast<Derived&>(*this);
			}

			Derived& setName(const std::string& name)
			{
				m_name = name;
				return static_cast<Derived&>(*this);
			}

		  protected:
			bool m_readable{false};
			bool m_writable{false};
			std::string m_name{""};
		};

	  public:
		virtual ~Attribute() = default;

	  protected:
		Attribute() = default;

	  public:
		/*----------------------*/
		/*-------Getters--------*/
		/*----------------------*/
		template <typename Type>
		Type getData() const
		{
			try
			{
				if (m_isConnected)
				{
					return std::any_cast<Type>(m_connectedAttribute->getData<Type>());
				}

				return std::any_cast<Type>(m_data);
			}
			catch (const std::bad_any_cast&)
			{
				// Handle error or assert
				assert(false && "Bad any_cast in getData()");
				throw std::bad_any_cast();
			}
		}

		std::string getName() const;
		bool isReadable() const;
		bool isWritable() const;

		/*----------------------*/
		/*-------Setters--------*/
		/*----------------------*/
		template <typename Type>
		void setData(const Type& data);

		void setName(const std::string& name);
		void setReadable(bool state);
		void setWritable(bool state);

	  private:
		std::any m_data;
		std::shared_ptr<Attribute> m_connectedAttribute;
		std::string m_name;
		bool m_readable;
		bool m_writable;
		bool m_array;
		bool m_isConnected;
	};

	// TODO Attribute need redesign
	template <typename Type>
	inline void Attribute::setData(const Type& data)
	{
		m_data = data;
		m_isConnected = false;
	}

	template <>
	inline void Attribute::setData(const std::shared_ptr<Attribute>& data)
	{
		m_connectedAttribute = data;
		m_isConnected = true;
	}

	/*----------------------*/
	/*-------Handlers-------*/
	/*----------------------*/
	class AttributeHandler
	{
	  public:
		virtual ~AttributeHandler() = default;

		//virtual std::shared_ptr<Attribute> getAttribute() = 0;
	};

} // namespace st::core

#endif // ST_CORE_NODES_NODE_HPP