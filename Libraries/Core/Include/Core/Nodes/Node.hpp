#ifndef ST_CORE_NODES_NODE_HPP
#define ST_CORE_NODES_NODE_HPP

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
	template <typename T>
	concept NumericAttribute = std::is_same<T, bool>::value ||
							   std::is_same<T, int32_t>::value ||
							   std::is_same<T, uint32_t>::value ||
							   std::is_same<T, float>::value ||
							   std::is_same<T, double>::value;

	using NumericAttributeData = std::variant<bool,
											  int32_t,
											  uint32_t,
											  float,
											  double>;

	using NumericAttributeDataArray = std::variant<std::vector<bool>,
												   std::vector<int32_t>,
												   std::vector<uint32_t>,
												   std::vector<float>,
												   std::vector<double>>;

	enum class Type
	{
		Unknown = 0,
		Bool,
		Int,
		UInt,
		Float,
		Double,
		Vector2f,
		Vector3f,
		Vector4f,
		Matrix2f,
		Matrix3f,
		Matrix4f,

		MeshData
	};

	// Function that convert Type to enum
	template <typename T>
	constexpr Type convertTypeToEnum()
	{
		if (std::is_same<T, int32_t>::value)
		{
			return Type::Int;
		}
		else if (std::is_same<T, uint32_t>::value)
		{
			return Type::UInt;
		}
		else if (std::is_same<T, float>::value)
		{
			return Type::Float;
		}
		else if (std::is_same<T, double>::value)
		{
			return Type::Double;
		}
		else if (std::is_same<T, bool>::value)
		{
			return Type::Bool;
		}
		else if (std::is_same<T, MeshData>::value)
		{
			return Type::MeshData;
		}
		// else if(std::is_same<T, Eigen::Vector2f>::value)
		//{
		//     return Type::Vector2f;
		// }
		// else if(std::is_same<T, Eigen::Vector3f>::value)
		//{
		//     return Type::Vector3f;
		// }
		// else if(std::is_same<T, Eigen::Vector4f>::value)
		//{
		//     return Type::Vector4f;
		// }
		// else if(std::is_same<T, Eigen::Matrix2f>::value)
		//{
		//     return Type::Matrix2f;
		// }
		// else if(std::is_same<T, Eigen::Matrix3f>::value)
		//{
		//     return Type::Matrix3f;
		// }
		// else if(std::is_same<T, Eigen::Matrix4f>::value)
		//{
		//     return Type::Matrix4f;
		// }
		else
		{
			assert(false && "Type not supported");
		}
	}

	class AttributeDescriptor;

	class AttributeBase
	{
	  public:
		AttributeBase() = default;

		AttributeBase(Type type) :
			m_type(type)
		{
		}

		AttributeBase(const std::string& name) :
			m_name(name)
		{
		}

		virtual ~AttributeBase() = default;

		Type getType() const
		{
			return m_type;
		}

		const std::string& getName() const
		{
			return m_name;
		}

		void setName(const std::string& name)
		{
			m_name = name;
		}

		bool isReadable()
		{
			return m_readable;
		}

		bool isWritable()
		{
			return m_writable;
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
		Type m_type;
		std::string m_name;
		bool m_readable;
		bool m_writable;
	};

	template <typename T>
	class Attribute : public AttributeBase
	{
	  public:
		Attribute() :
			AttributeBase(Type::Unknown),
			m_value(T())
		{
		}

		Attribute(T value) :
			AttributeBase(Type::Unknown),
			m_value(value)
		{
		}

		Attribute(const std::string& name) :
			AttributeBase(name)
		{
		}

		std::variant<T, Attribute<T>*> getValue() const
		{
			return m_value;
		}

	  private:
		std::variant<T, Attribute<T>*> m_value2;
		T m_value;
	};

template<typename Derived>
	class AttributeBuilder
	{
	  public:
		AttributeBuilder(const std::string& name) :
			m_name(name)
		{
		}

		Derived& setReadable(bool state)
		{
			m_readable = state;
			return *this;
		}

		Derived& setWritable(bool state)
		{
			m_writable = state;
			return *this;
		}

		std::shared_ptr<AttributeBase> build()
		{
			return std::make_shared<AttributeBase>(m_name);
		}

	  private:
		std::string m_name;
		bool m_readable;
		bool m_writable;
	};


    template<typename T>
    class NumericAttributeBuilder : public AttributeBuilder
    {
        public:
        NumericAttributeBuilder(const std::string& name) :
            AttributeBuilder(name)
        {
        }

        void setDefaultValue(T value)
        {
            m_defaultValue = value;
        }

        void setLimits(T min, T max)
        {
            m_min = min;
            m_max = max;
        }

        void setSoftLimits(T min, T max)
        {
            m_softMin = min;
            m_softMax = max;
        }

        private:    
        T m_defaultValue;
        T m_min;
        T m_max;
        T m_softMin;
        T m_softMax;

    };



	template <typename T>
	class Output : public Attribute<T>
	{
	  public:
		Output() :
			Attribute<T>(T())
		{
			this->setWritable(false);
			this->setReadable(true);
		}

		Output(T value) :
			Attribute<T>(value)
		{
		}
	};

	// TODO Input is hanlde to attribute not attribute itself
	template <typename T>
	class Input : public Attribute<T>
	{
	  public:
		Input() :
			Attribute<T>()
		{
			this->setWritable(true);
			this->setReadable(false);
		}

		Input(T value) :
			Attribute<T>(value)
		{
		}
	};

	/**
	 * @brief NumericInputHandler is a class that provide handler over numeric attributes
	 * for easy access and manipulation on generic attributes
	 */
	template <typename T> // TODO add Constraint
	class NumericInputHandler
	{
	  public:
		NumericInputHandler(std::shared_ptr<AttributeBase> attribute) :
			m_attribute(attribute)
		{
		}

		operator T () const
		{
			return std::get<T>(m_attribute->getValue());
		}

		NumericInputHandler<T>& operator= (const T& rhs)
		{
			m_attribute->setValue(rhs);
			return *this;
		}




        private :
        std::shared_ptr<AttributeBase> m_attribute;
	};

	class Handler
	{
	  public:
		Handler(AttributeBase* attribute) :
			m_attribute(attribute)
		{
		}
		virtual ~Handler() = default;

		std::string getName() const
		{
			return m_attribute->getName();
		}

		bool isReadable()
		{
			return m_attribute->isReadable();
		}

		bool isWritable()
		{
			return m_attribute->isWritable();
		}

		AttributeBase* getAttribute() const
		{
			return m_attribute;
		}

	  protected: // TODO refactor to private?
		AttributeBase* m_attribute;
	};

	class InputHandler : public Handler
	{
	  public:
		template <typename T>
		InputHandler(Input<T>* input) :
			Handler(input)
		{
		}

		template <typename T>
		const T& getInput() const
		{
			Input<T>* input = dynamic_cast<Input<T>*>(m_attribute);
			return *input;
		}
	};

	class OutputHandler : public Handler
	{
	  public:
		template <typename T>
		OutputHandler(Output<T>* output) :
			Handler(output)
		{
		}

		template <typename T>
		const T& getOutput() const
		{
			Output<T>* output = dynamic_cast<Output<T>*>(m_attribute);
			return *output;
		}
	};

	template <typename Type>
	class NumericHandle
	{
	};

	class Node2
	{
	  public:
		virtual ~Node2() = default;

		void defineNode(const std::string& name)
		{
			m_name = name;
		}

		void addAttribute(std::shared_ptr<AttributeBase> attribute)
		{
			m_attributes.push_back(attribute);
		}

		template <typename T>
		void addInput(Input<T>& input)
		{
			// m_attributes.push_back(input); //TODO refactor
		}

		template <typename T>
		void addOutput(Output<T>& output)
		{
			// m_attributes.push_back(output); //TODO refactor
		}

		virtual bool initialize() = 0;
		virtual bool compute() = 0;

		std::string getName() const
		{
			return m_name;
		}

		std::vector<std::shared_ptr<AttributeBase>> getAttributes() const
		{
			return m_attributes;
		}

	  private:
		std::string m_name;
		std::vector<std::shared_ptr<AttributeBase>> m_attributes;
	};

	class Connection
	{
	  public:
		Connection(std::shared_ptr<Node2> sourceNode,
				   std::shared_ptr<AttributeBase> sourceAttrName,
				   std::shared_ptr<Node2> targetNode,
				   std::shared_ptr<AttributeBase> targetAttrName) :
			sourceNode(sourceNode),
			sourceAttrName(sourceAttrName),
			targetNode(targetNode),
			targetAttrName(targetAttrName)
		{
		}

		std::shared_ptr<Node2> sourceNode;
		std::shared_ptr<AttributeBase> sourceAttrName;

		std::shared_ptr<Node2> targetNode;
		std::shared_ptr<AttributeBase> targetAttrName;
	};

	class NodeGraph
	{
	  public:
		void addNode(std::shared_ptr<Node2> node)
		{
			m_nodes.push_back(node);
		}

		void addConnection(std::shared_ptr<Node2> sourceNode,
						   std::shared_ptr<AttributeBase> sourceAttrName,
						   std::shared_ptr<Node2> targetNode,
						   std::shared_ptr<AttributeBase> targetAttrName)
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