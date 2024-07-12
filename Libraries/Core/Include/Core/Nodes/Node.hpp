#ifndef ST_CORE_NODES_NODE_HPP
#define ST_CORE_NODES_NODE_HPP



#include <any>
#include <cassert>
#include <memory>
#include <optional>
#include <print>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <functional>

namespace st::core
{
	template <typename Type>
	concept NumericAttributeType = std::is_same<Type, bool>::value ||
								   std::is_same<Type, int32_t>::value ||
								   std::is_same<Type, uint32_t>::value ||
								   std::is_same<Type, float>::value ||
								   std::is_same<Type, double>::value;

	/*----------------------*/
	/*-------Attributes-----*/
	/*----------------------*/
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

			Derived& setReadable(bool state)
			{
				m_readable = state;
				return static_cast<Derived&>(*this);
			}

			Derived& setWritable(bool state)
			{
				m_writable = state;
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
		Type getValue() const
		{
			try
			{
				return std::any_cast<Type>(m_data);
			}
			catch (const std::bad_any_cast&)
			{
				// Handle error or assert
				assert(false && "Bad any_cast in getValue()");
				throw std::bad_any_cast();
			}
		}

		const std::string getName() const
		{
			return m_name;
		}

		bool isReadable()
		{
			return m_readable;
		}

		bool isWritable()
		{
			return m_writable;
		}

		/*----------------------*/
		/*-------Setters--------*/
		/*----------------------*/
		template <typename Type>
		void setData(const Type& data)
		{
			m_data = data;
		}

		void setName(const std::string& name)
		{
			m_name = name;
		}

		void setReadable(bool state)
		{
			m_readable = state;
		}

		void setWritable(bool state)
		{
			m_writable = state;
		}

	  private:
		std::any m_data;
		std::string m_name;
		bool m_readable;
		bool m_writable;
		bool m_array;
	};

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
				numericAttribute->setData<Type>(m_defaultValue);
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
		Type getValue() const
		{
			return Attribute::getValue<Type>();
		}

		void setValue(Type value)
		{
			setData<Type>(value);
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
				typedAttribute->setData<Type>(m_defaultValue);

				return typedAttribute;
			}

		  private:
			Type m_defaultValue;
		};

		friend class Builder;
		~TypedAttribute() = default;

	  protected:
		TypedAttribute() = default;

	  public:
		Type getValue() const
		{
			return m_value;
		}

		void setValue(Type value)
		{
			m_value = value;
		}

	  private:
		Type m_value;
	};

	/*----------------------*/
	/*-------Handlers-------*/
	/*----------------------*/
	class Handler
	{
	};

	/**
	 * @brief NumericInputHandler is a class that provide handler over numeric attributes
	 * for easy access and manipulation on generic attributes
	 *
	 * Inputs are always readable and writable
	 */
	template <NumericAttributeType Type> // TODO add Constraint
	class NumericInputHandler : public Handler
	{
	  public:
		NumericInputHandler() = default;

		NumericInputHandler(std::shared_ptr<NumericAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		operator Type () const
		{

			return m_attribute->getValue();
		}

		NumericInputHandler<Type>& operator= (const Type& rhs)
		{
			m_attribute->setValue(rhs);
			return *this;
		}

		NumericInputHandler& operator+= (const Type& rhs)
		{
			void* data = m_attribute->getData();
			Type* value = static_cast<Type*>(data);
			*value += rhs;
			return *this;
		}

		NumericInputHandler& operator-= (const Type& rhs)
		{
			void* data = m_attribute->getData();
			Type* value = static_cast<Type*>(data);
			*value -= rhs;
			return *this;
		}

		NumericInputHandler& operator*= (const Type& rhs)
		{
			void* data = m_attribute->getData();
			Type* value = static_cast<Type*>(data);
			*value *= rhs;
			return *this;
		}

		NumericInputHandler& operator/= (const Type& rhs)
		{
			void* data = m_attribute->getData();
			Type* value = static_cast<Type*>(data);
			*value /= rhs;
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
	class NumericOutputHandler : public Handler
	{
	  public:
		NumericOutputHandler() = default;

		NumericOutputHandler(std::shared_ptr<NumericAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		Type getValue() const
		{
			return m_attribute->getValue();
		}

		std::shared_ptr<NumericAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<NumericAttribute<Type>> m_attribute;
	};

	template <typename Type>
	class TypedInputHandler : public Handler
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

		TypedInputHandler<Type>& operator= (const Type& rhs)
		{
			m_attribute->setValue(rhs);
			return *this;
		}

		std::shared_ptr<TypedAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<TypedAttribute<Type>> m_attribute;
	};

	template <typename Type>
	class TypedOutputHandler : public Handler
	{
	  public:
		TypedOutputHandler() = default;

		TypedOutputHandler(std::shared_ptr<TypedAttribute<Type>> attribute) :
			m_attribute(attribute)
		{
		}

		operator Type () const
		{
			return m_attribute->getValue();
		}

		TypedOutputHandler<Type>& operator= (const Type& rhs)
		{
			assert(m_attribute && "Attribute is not initialized");
			m_attribute->setValue(rhs);
			return *this;
		}

		Type getValue() const
		{
			return m_attribute->getValue();
		}

		std::shared_ptr<TypedAttribute<Type>> getAttribute() const
		{
			return m_attribute;
		}

	  private:
		std::shared_ptr<TypedAttribute<Type>> m_attribute;
	};

	/*----------------------*/
	/*-------Node----------*/
	/*----------------------*/

	class Node
	{
	  public:
		enum class NodeState
		{
			eUninitialized,
			eInitialized,
			eDirty,
			eClean

		};

		Node() = default;
		virtual ~Node() = default;

		void defineNode(const std::string& name)
		{
			m_name = name;
		}

		void addAttribute(std::shared_ptr<Attribute> attribute)
		{
			m_attributes.push_back(attribute);
		}

		template <typename Type>
		void addAttribute(NumericInputHandler<Type> attribute)
		{
			m_attributes.push_back(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(NumericOutputHandler<Type> attribute)
		{
			m_attributes.push_back(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(TypedInputHandler<Type> attribute)
		{
			m_attributes.push_back(attribute.getAttribute());
		}

		template <typename Type>
		void addAttribute(TypedOutputHandler<Type> attribute)
		{
			m_attributes.push_back(attribute.getAttribute());
		}

		virtual bool initialize() = 0;
		virtual bool compute() = 0;


		std::string getName() const
		{
			return m_name;
		}

		std::vector<std::shared_ptr<Attribute>> getAttributes() const
		{
			return m_attributes;
		}

		NodeState getState() const
		{
			return m_state;
		}

	  private:
		std::string m_name;
		std::vector<std::shared_ptr<Attribute>> m_attributes;
		NodeState m_state;
	};

	class Connection
	{
	  public:
		Connection(std::shared_ptr<Node> sourceNode,
				   std::shared_ptr<Attribute> sourceAttrName,
				   std::shared_ptr<Node> targetNode,
				   std::shared_ptr<Attribute> targetAttrName) :
			sourceNode(sourceNode),
			sourceAttrName(sourceAttrName),
			targetNode(targetNode),
			targetAttrName(targetAttrName)
		{
		}

		std::shared_ptr<Node> sourceNode;
		std::shared_ptr<Attribute> sourceAttrName;

		std::shared_ptr<Node> targetNode;
		std::shared_ptr<Attribute> targetAttrName;
	};






	template <typename NodeType>
	class NodeFactory
	{
		public:
		static std::shared_ptr<Node> createNode()
		{
			return std::make_shared<NodeType>();
		}
	};

	template <typename NodeType>
	class NodeInitializer
	{
		public:
		static bool initializeNode()
		{
			return true;
		}
	};


	class NodeRegistry
	{

		using NodeCreator = std::function<std::shared_ptr<Node>()>;
		using NodeInitializer = std::function<bool()>;

		public:
		static NodeRegistry& getInstance()
		{
			static NodeRegistry instance;
			return instance;
		}

		void registerNode(const std::string& name, NodeCreator creator, NodeInitializer initializer)
		{
			m_nodes[name] = creator;
			m_nodeInitializers.push_back(initializer);
		}


		std::shared_ptr<Node> createNode(const std::string& name)
		{
			auto it = m_nodes.find(name);
			if (it != m_nodes.end())
			{
				return it->second();
			}
			else
			{
				return nullptr;
			}
		}

		std::vector<NodeInitializer> getNodeInitializers() const
		{
			return m_nodeInitializers;
		}


		private:
		NodeRegistry() = default;



		std::unordered_map<std::string, NodeCreator> m_nodes;
		std::vector<NodeInitializer> m_nodeInitializers;
	};




	template <typename NodeType>
	class RegisterNode
	{
	  public:
		RegisterNode(const std::string& name)
		{
			static_assert(std::is_base_of<Node, NodeType>::value, "NodeType must derive from Node");

			NodeRegistry::getInstance().registerNode(name, &NodeFactory<NodeType>::createNode, &NodeInitializer<NodeType>::initializeNode);
		}

		//stati
	};

} // namespace st::core

#endif // ST_CORE_NODES_NODE_HPP