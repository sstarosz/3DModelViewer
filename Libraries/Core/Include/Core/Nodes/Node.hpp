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

#include "../Mesh.hpp" //TODO clean up

/*
Supported types:

- int
- float
- double
- bool
//- std::string (Optional)

- Eigen::Vector2f
- Eigen::Vector3f
- Eigen::Vector4f

- Eigen::Matrix2f
- Eigen::Matrix3f
- Eigen::Matrix4f

Arrays
*/

namespace st::core
{
	template <typename Type>
	concept NumericAttributeType = std::is_same<Type, bool>::value ||
								   std::is_same<Type, int32_t>::value ||
								   std::is_same<Type, uint32_t>::value ||
								   std::is_same<Type, float>::value ||
								   std::is_same<Type, double>::value;

	enum class Type
	{
		eUnknown = 0,
		eBool,
		eInt32,
		eUInt,
		eFloat,
		eDouble,
		eVector2f,
		eVector3f,
		eVector4f,
		eMatrix2f,
		eMatrix3f,
		eMatrix4f,

		MeshData
	};

	// Function that convert Type to enum
	template <typename Type>
	constexpr Type convertTypeToEnum()
	{
		if (std::is_same<Type, int32_t>::value)
		{
			return Type::Int;
		}
		else if (std::is_same<Type, uint32_t>::value)
		{
			return Type::UInt;
		}
		else if (std::is_same<Type, float>::value)
		{
			return Type::Float;
		}
		else if (std::is_same<Type, double>::value)
		{
			return Type::Double;
		}
		else if (std::is_same<Type, bool>::value)
		{
			return Type::Bool;
		}
		else if (std::is_same<Type, MeshData>::value)
		{
			return Type::MeshData;
		}
		// else if(std::is_same<Type, Eigen::Vector2f>::value)
		//{
		//     return Type::Vector2f;
		// }
		// else if(std::is_same<Type, Eigen::Vector3f>::value)
		//{
		//     return Type::Vector3f;
		// }
		// else if(std::is_same<Type, Eigen::Vector4f>::value)
		//{
		//     return Type::Vector4f;
		// }
		// else if(std::is_same<Type, Eigen::Matrix2f>::value)
		//{
		//     return Type::Matrix2f;
		// }
		// else if(std::is_same<Type, Eigen::Matrix3f>::value)
		//{
		//     return Type::Matrix3f;
		// }
		// else if(std::is_same<Type, Eigen::Matrix4f>::value)
		//{
		//     return Type::Matrix4f;
		// }
		else
		{
			assert(false && "Type not supported");
		}
	}

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

		Type getType() const
		{
			return m_type;
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
		Type m_type;
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
		class Handler2
		{
		};

		/**
		 * @brief NumericInputHandler is a class that provide handler over numeric attributes
		 * for easy access and manipulation on generic attributes
		 *
		 * Inputs are always readable and writable
		 */
		template <NumericAttributeType Type> // TODO add Constraint
		class NumericInputHandler : public Handler2
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
		class NumericOutputHandler : public Handler2
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
		class TypedInputHandler : public Handler2
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
		class TypedOutputHandler : public Handler2
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


		// TODO Old API refactor
		template <typename Type>
		class Output : public Attribute
		{
		  public:
			Output() :
				Attribute()
			{
				this->setWritable(false);
				this->setReadable(true);
			}

			Output(Type value)
			{
			}
		};

		// TODO Input is hanlde to attribute not attribute itself
		template <typename Type>
		class Input : public Attribute
		{
		  public:
			Input()
			{
				this->setWritable(true);
				this->setReadable(false);
			}

			Input(Type value)
			{
			}
		};

		class Handler
		{
		  public:
			Handler(Attribute* attribute) :
				m_attribute(attribute)
			{
			}
			virtual ~Handler() = default;

			std::string getName() const
			{
				// return m_attribute->getName();
				return "";
			}

			bool isReadable()
			{
				// return m_attribute->isReadable();
				return false;
			}

			bool isWritable()
			{
				// return m_attribute->isWritable();
				return false;
			}

			Attribute* getAttribute() const
			{
				return m_attribute;
			}

		  protected: // TODO refactor to private?
			Attribute* m_attribute;
		};

		class InputHandler : public Handler
		{
		  public:
			template <typename Type>
			InputHandler(Input<Type>* input) :
				Handler(input)
			{
			}

			template <typename Type>
			const Type& getInput() const
			{
				Input<Type>* input = dynamic_cast<Input<Type>*>(m_attribute);
				return *input;
			}
		};

		class OutputHandler : public Handler
		{
		  public:
			template <typename Type>
			OutputHandler(Output<Type>* output) :
				Handler(output)
			{
			}

			template <typename Type>
			const Type& getOutput() const
			{
				Output<Type>* output = dynamic_cast<Output<Type>*>(m_attribute);
				return *output;
			}
		};

		class Node2
		{
		  public:
			Node2() = default;
			virtual ~Node2() = default;

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

			template <typename Type>
			void addInput(Input<Type>& input)
			{
				// m_attributes.push_back(input); //TODO refactor
			}

			template <typename Type>
			void addOutput(Output<Type>& output)
			{
				// m_attributes.push_back(output); //TODO refactor
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

		  private:
			std::string m_name;
			std::vector<std::shared_ptr<Attribute>> m_attributes;
		};

		class Connection
		{
		  public:
			Connection(std::shared_ptr<Node2> sourceNode,
					   std::shared_ptr<Attribute> sourceAttrName,
					   std::shared_ptr<Node2> targetNode,
					   std::shared_ptr<Attribute> targetAttrName) :
				sourceNode(sourceNode),
				sourceAttrName(sourceAttrName),
				targetNode(targetNode),
				targetAttrName(targetAttrName)
			{
			}

			std::shared_ptr<Node2> sourceNode;
			std::shared_ptr<Attribute> sourceAttrName;

			std::shared_ptr<Node2> targetNode;
			std::shared_ptr<Attribute> targetAttrName;
		};

		class NodeGraph
		{
		  public:
			void addNode(std::shared_ptr<Node2> node)
			{
				m_nodes.push_back(node);
			}

			void addConnection(std::shared_ptr<Node2> sourceNode,
							   std::shared_ptr<Attribute> sourceAttrName,
							   std::shared_ptr<Node2> targetNode,
							   std::shared_ptr<Attribute> targetAttrName)
			{
				// TODO

				// Check if sourceAttribute is readable
				// Check if destinationAttribute is writable
				// Check if sourceAttribute and destinationAttribute are compatible (type)
				// Check if sourceAttribute and destinationAttribute are compatible (dimension)

				std::println("Adding connection");
				m_connections.emplace_back(std::make_shared<Connection>(sourceNode, sourceAttrName, targetNode, targetAttrName));
			}

			void evaluate()
			{
			}

			std::vector<std::shared_ptr<Node2>> getNodes() const
			{
				return m_nodes;
			}

			std::vector<std::shared_ptr<Connection>> getConnections() const
			{
				return m_connections;
			}

			std::vector<std::shared_ptr<Node2>> m_nodes;
			std::vector<std::shared_ptr<Connection>> m_connections;
		};

		template <typename NodeType>
		class RegisterNode
		{
		  public:
			RegisterNode()
			{
				static_assert(std::is_base_of<Node2, NodeType>::value, "NodeType must derive from Node2");
			}

			// NodeType* getNode() const
			//{
			//     return m_node;
			// }
			//
			// private:
			// std::map<std::string, NodeType*> m_nodes;
		};

	} // namespace st::core

#endif // ST_CORE_NODES_NODE_HPP