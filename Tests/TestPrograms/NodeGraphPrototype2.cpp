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

namespace old
{

	enum class DataType
	{
		eUnknown = 0,
		eBool,
		eInt,
		eUInt,
		eFloat,
		eDouble,
		eArray,
		eAttribute,
		eTypeCount
	};

	std::string toString(DataType type)
	{
		switch (type)
		{
		case DataType::eBool:
			return "bool";
		case DataType::eInt:
			return "int";
		case DataType::eUInt:
			return "uint";
		case DataType::eFloat:
			return "float";
		case DataType::eDouble:
			return "double";
		case DataType::eArray:
			return "array";
		case DataType::eAttribute:
			return "attribute";
		default:
			return "unknown";
		}
	}

	enum class ComponentCount
	{
		eUnknown = 0,
		eScalar = 1,
		eVec2 = 2,
		eVec3 = 3,
		eVec4 = 4,
		eMat2 = 4,
		eMat3 = 9,
		eMat4 = 16,
		eTypeCount
	};

	/**
	 * @brief Class that contains runtime type information about type
	 */
	class TypeInfo
	{
	  public:
		TypeInfo() = default;
		TypeInfo(DataType baseType,
				 ComponentCount componentCount) :
			m_baseType(baseType),
			m_componentCount(componentCount)
		{
		}

		DataType getBaseType() const
		{
			return m_baseType;
		}

		std::string getBaseTypeString() const
		{
			return toString(m_baseType);
		}

		ComponentCount getComponentCount() const
		{
			return m_componentCount;
		}

		int32_t getComponentCountValue() const
		{
			return std::to_underlying(m_componentCount);
		}

	  private:
		DataType m_baseType;
		ComponentCount m_componentCount;
	};

	template <typename T>
	struct is_basic_type
	{
		static constexpr bool value = std::is_same_v<T, bool> ||
									  std::is_same_v<T, int> ||
									  std::is_same_v<T, uint32_t> ||
									  std::is_same_v<T, float> ||
									  std::is_same_v<T, double>;
	};

	template <typename T>
	constexpr ComponentCount getComponentCount()
	{
		if constexpr (is_basic_type<T>::value)
		{
			return ComponentCount::eScalar;
		}
		else if constexpr (std::is_same_v<T, Eigen::Vector2f>)
		{
			return ComponentCount::eVec2;
		}
		else if constexpr (std::is_same_v<T, Eigen::Vector3f>)
		{
			return ComponentCount::eVec3;
		}
		else if constexpr (std::is_same_v<T, Eigen::Vector4f>)
		{
			return ComponentCount::eVec4;
		}
		else if constexpr (std::is_same_v<T, Eigen::Matrix2f>)
		{
			return ComponentCount::eMat2;
		}
		else if constexpr (std::is_same_v<T, Eigen::Matrix3f>)
		{
			return ComponentCount::eMat3;
		}
		else if constexpr (std::is_same_v<T, Eigen::Matrix4f>)
		{
			return ComponentCount::eMat4;
		}
		else if constexpr (std::is_same_v<T, std::vector<T>>)
		{
			return ComponentCount::eScalar;
		}
		else
		{
			return ComponentCount::eUnknown;
		}
	}

	template <typename T>
	constexpr TypeInfo getTypeId()
	{
		constexpr DataType type = std::is_same_v<T, bool> ? DataType::eBool : std::is_same_v<T, int>		   ? DataType::eInt
																		  : std::is_same_v<T, uint32_t>		   ? DataType::eUInt
																		  : std::is_same_v<T, float>		   ? DataType::eFloat
																		  : std::is_same_v<T, double>		   ? DataType::eDouble
																		  : std::is_same_v<T, Eigen::Vector2f> ? DataType::eFloat
																		  : std::is_same_v<T, Eigen::Vector3f> ? DataType::eFloat
																		  : std::is_same_v<T, Eigen::Vector4f> ? DataType::eFloat
																		  : std::is_same_v<T, Eigen::Matrix2f> ? DataType::eFloat
																		  : std::is_same_v<T, Eigen::Matrix3f> ? DataType::eFloat
																		  : std::is_same_v<T, Eigen::Matrix4f> ? DataType::eFloat
																		  : std::is_same_v<T, std::vector<T>>  ? DataType::eArray
																											   : DataType::eUnknown;

		return TypeInfo(type, getComponentCount<T>());
	}

	enum class eAttributeType
	{
		eUnknown = 0,
		eInput,
		eOutput,
		eTypeCount
	};

	class AttributeBase
	{
	  public:
		virtual ~AttributeBase() = default;

		virtual TypeInfo getTypeInfo() const = 0;
	};

	template <eAttributeType AttributeType, typename StoredType>
	class Attribute : public AttributeBase
	{
		constexpr static bool isReadOnly = AttributeType == eAttributeType::eInput;

		using PointerType = StoredType*;
		using ConstPointerType = const StoredType*;
		using DataType = StoredType;
		using ConstDataType = const StoredType;

		using Data = std::conditional_t<isReadOnly, PointerType, DataType>;

		using DefaultDataValue = std::conditional_t<isReadOnly, ConstDataType, DataType>;

	  public:
		Attribute() :
			m_value(),
			m_defaultValue(DataType()),
			m_typeInfo(getTypeId<DataType>())
		{
			if constexpr (isReadOnly)
			{
				m_value = nullptr;
			}
		}

		virtual ~Attribute() = default;

		DataType& getValue()
		{
			if constexpr (isReadOnly)
			{
				throw std::runtime_error("Cannot modify read only attribute");
			}
			else
			{
				return m_value;
			}
		}

		const DataType& getValue() const
		{
			if constexpr (isReadOnly)
			{
				if (m_value == nullptr)
				{
					return m_defaultValue;
				}
				return *m_value;
			}
			else
			{
				return m_value;
			}
		}

		TypeInfo getTypeInfo() const override
		{
			return m_typeInfo;
		}

		Data m_value;
		DataType m_defaultValue;
		TypeInfo m_typeInfo;
	};

	template <typename OutputTypeT>
	class Output : public Attribute<eAttributeType::eOutput, OutputTypeT>
	{
		using OutputType = OutputTypeT;

	  public:
		Output() = default;

		const OutputType& operator() () const
		{
			return this->m_value;
		}

		OutputType& operator= (const OutputType& value)
		{
			this->m_value = value;
			return this->m_value;
		}
	};

	template <typename InputTypeT>
	class Input : public Attribute<eAttributeType::eInput, InputTypeT>
	{
	  public:
		using InputType = InputTypeT;

		const InputType& operator() () const
		{
			return this->getValue();
		}

		template <typename T>
		void setDefaultValue(const T& value)
		{
			this->m_defaultValue = value;
		}

		void setConnection(Attribute<eAttributeType::eOutput, InputType>& output)
		{
			this->m_value = &output.m_value;
		}
	};

	class Handler
	{
	  public:
		Handler(AttributeBase* attribute,
				const std::string& name) :
			m_attribute(attribute),
			m_name(name)
		{
		}

		virtual ~Handler() = default;

		AttributeBase* getAttribute() const
		{
			return m_attribute;
		}

		std::string getName() const
		{
			return m_name;
		}

		TypeInfo getTypeInfo() const
		{
			return m_attribute->getTypeInfo();
		}

	  private:
		AttributeBase* m_attribute;
		std::string m_name;
	};

	class OutputHandle : public Handler
	{
	  public:
		template <typename T>
		OutputHandle(Output<T>* output,
					 const std::string& name) :
			Handler(output, name)
		{
		}

		template <typename T>
		T getData() const
		{
			// Validate error handling
			Output<T>* output = dynamic_cast<Output<T>*>(this->getAttribute());
			return output->m_value;
		}
	};

	class InputHandle : public Handler
	{
	  public:
		template <typename T>
		InputHandle(Input<T>* input,
					const std::string& name) :
			Handler(input, name)
		{
		}

		template <typename T>
		const T& getData() const
		{
			// Validate error handling
			const Input<T>* input = dynamic_cast<const Input<T>*>(this->getAttribute());
			return input->getValue();
		}

		bool isConnected() const
		{
			// TODO check if the connection is valid
			return false;
			// return m_input != nullptr;
		}
	};

	class Node
	{
	  public:
		Node() = default;
		~Node() = default;

		virtual void compute() = 0;

		// Inputs to the node
		// Outputs from the node

		std::string getName() const
		{
			return m_name;
		}

		std::vector<InputHandle> getInputHandles()
		{
			return m_inputs;
		}

		virtual std::vector<OutputHandle> getOutputHandles()
		{
			return m_outputs;
		}

		template <typename Type>
		void registerInputs(Input<Type>& input, const std::string& name)
		{
			m_inputs.push_back(InputHandle(&input, name));
		}

		template <typename Type>
		void registerOutputs(Output<Type>& output, const std::string& name)
		{
			m_outputs.push_back(OutputHandle(&output, name));
		}

		// TODO add more node specific data
		void defineNode(const std::string& name)
		{
			m_name = name;
		}

	  private:
		std::string m_name;
		std::vector<InputHandle> m_inputs;
		std::vector<OutputHandle> m_outputs;
	};

	class AddNode : public Node
	{
	  public:
		AddNode()
		{
			defineNode("Add");

			registerInputs(m_inputs.input1, "input1");
			registerInputs(m_inputs.input2, "input2");

			registerOutputs(m_outputs.output1, "output1");
		}

		struct inputs
		{
			Input<uint32_t> input1;
			Input<uint32_t> input2;
		};

		struct outputs
		{
			Output<uint32_t> output1;
		};

		void compute() override
		{
			// Compute the output
			auto result = m_inputs.input1() + m_inputs.input2();

			// Set the output
			m_outputs.output1 = result;
		}

		inputs m_inputs;
		outputs m_outputs;
	};

	class MultiplyNode : public Node
	{
	  public:
		struct inputs
		{
			Input<uint32_t> input1;
			Input<uint32_t> input2;
		};

		struct outputs
		{
			Output<uint32_t> output1;
		};

		MultiplyNode()
		{
			defineNode("Multiply");

			registerInputs(m_inputs.input1, "input1");
			registerInputs(m_inputs.input2, "input2");

			registerOutputs(m_outputs.output1, "output1");
		}

		void compute() override
		{
			// Compute the output
			auto result = m_inputs.input1() * m_inputs.input2();

			// Set the output
			m_outputs.output1 = result;
		}

		inputs m_inputs;
		outputs m_outputs;
	};

	class NodeDefinition
	{
	  public:
		NodeDefinition() = default;

		static void initialize()
		{
		}

	  private:
		// static std::vector<InputsAtributes> m_inputs;
		// static std::vector<OutputAtributes> m_outputs;
	};

	class NodeGraph
	{
	  public:
		class Connection
		{
		  public:
			Connection(std::weak_ptr<Node> nodeConnectedFrom,
					   const OutputHandle outputHandle,
					   std::weak_ptr<Node> nodeConnectedTo,
					   const InputHandle inputHandle) :

				m_nodeConnectedTo(nodeConnectedTo),
				m_nodeConnectedFrom(nodeConnectedFrom),
				m_inputHandle(inputHandle),
				m_outputHandle(outputHandle)
			{
			}

			bool operator== (const Connection& other) const
			{
				return m_nodeConnectedFrom.lock() == other.m_nodeConnectedFrom.lock() &&
					   m_nodeConnectedTo.lock() == other.m_nodeConnectedTo.lock() &&
					   m_inputHandle.getName() == other.m_inputHandle.getName() &&
					   m_outputHandle.getName() == other.m_outputHandle.getName();
			}

			std::weak_ptr<Node> m_nodeConnectedTo;
			std::weak_ptr<Node> m_nodeConnectedFrom;

			InputHandle m_inputHandle;
			OutputHandle m_outputHandle;
		};

	  public:
		NodeGraph() = default;
		~NodeGraph() = default;

		template <typename NodeType>
		std::shared_ptr<NodeType> addNode(std::shared_ptr<NodeType> node)
		{
			static_assert(std::is_base_of<Node, NodeType>::value, "Node must be derived from Node class");

			m_nodes.push_back(node);

			return node;
		}

		void removeNode(std::shared_ptr<Node> node)
		{
			// First remove all the connections from and to the node
			for (auto it = m_connections.begin(); it != m_connections.end();)
			{
				if (it->m_nodeConnectedFrom.lock() == node || it->m_nodeConnectedTo.lock() == node)
				{
					it = m_connections.erase(it);
				}
				else
				{
					++it;
				}
			}

			auto it = std::find(m_nodes.begin(), m_nodes.end(), node);
			if (it != m_nodes.end())
			{
				m_nodes.erase(it);
			}
		}

		void connectNodes(std::weak_ptr<Node> nodeFrom,
						  const OutputHandle outputHandle,
						  std::weak_ptr<Node> nodeTo,
						  const InputHandle inputHandle)
		{
			// connection(nodeFrom, outputHandle, nodeTo, inputHandle);

			Connection connection{nodeFrom, outputHandle, nodeTo, inputHandle};

			m_connections.push_back(connection);
		}

		void disconnectNodes(std::weak_ptr<Node> nodeFrom,
							 const OutputHandle outputHandle,
							 std::weak_ptr<Node> nodeTo,
							 const InputHandle inputHandle)
		{
			Connection connection{nodeFrom, outputHandle, nodeTo, inputHandle};

			auto it = std::find(m_connections.begin(), m_connections.end(), connection);
			if (it != m_connections.end())
			{
				m_connections.erase(it);
			}
		}

		std::vector<std::shared_ptr<Node>> getNodes() const
		{
			return m_nodes;
		}

		std::vector<Connection> getConnections() const
		{
			return m_connections;
		}

	  private:
		template <typename Type1, typename Type2>
		void connect(Output<Type1>& output, Input<Type2>& input)
		{
			static_assert(std::is_same<Type1, Type2>::value, "Types must be the same");
			// node1Attribute.
			// input.setConnection(&output.m_value);
			input.setConnection(output);
		}

		std::vector<std::shared_ptr<Node>> m_nodes;
		std::vector<Connection> m_connections;
	};

} // namespace old

template <typename Type>
class NumericHandle
{
  public:
	// Overloaded addition operator
	NumericHandle operator+ (const NumericHandle& other) const
	{
		return NumericHandle(m_value + other.m_value);
	}

	// Overloaded subtraction operator
	NumericHandle operator- (const NumericHandle& other) const
	{
		return NumericHandle(m_value - other.m_value);
	}

	// Overloaded multiplication operator
	NumericHandle operator* (const NumericHandle& other) const
	{
		return NumericHandle(m_value * other.m_value);
	}

	// Overloaded division operator
	NumericHandle operator/ (const NumericHandle& other) const
	{
		return NumericHandle(m_value / other.m_value);
	}

	// Overloaded assignment operator
	NumericHandle& operator= (const NumericHandle& other)
	{
		if (this != &other)
		{ // Avoid self-assignment
			this->m_value = other.m_value;
		}
		return *this;
	}

	// Constructor
	NumericHandle(Type value = Type()) :
		m_value(value)
	{
	}

  protected:
	Type m_value;
};

template <typename Type>
class NumericInput : public NumericHandle<Type>
{
  public:
	Type& operator() ()
	{
		return this->m_value;
	}
};

template <typename Type>
class NumericOutput : public NumericHandle<Type>
{
  public:
	using NumericHandle<Type>::NumericHandle;
	using NumericHandle<Type>::operator=;

	NumericOutput() = default;

	Type& operator() ()
	{
		return this->m_value;
	}
};

class Node
{

  public:
	virtual ~Node() = default;
	virtual std::string getName() const = 0;
	virtual void compute() = 0;
};



class NodeMultiInput : public Node
{
  public:
	virtual ~NodeMultiInput() = default;

	virtual std::string getName() const override
	{
		return "NodeMultiInput";
	}

	void compute() override
	{
		// Compute the output

		m_outputs.output = m_inputs.input1 * m_inputs.input2;
	}

  public: //TODO make private
	struct Input
	{
		std::string name;
		NumericInput<int32_t> input1;
		NumericInput<int32_t> input2;
	} m_inputs;

	struct Output
	{
		std::string name;
		NumericOutput<int32_t> output;
	} m_outputs;

};

template<>
struct std::formatter<std::shared_ptr<Node>>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        // No parsing logic needed for this simple formatter
        return ctx.begin();
    }

    // Format function for std::shared_ptr<Node>
    template <typename FormatContext>
    auto format(const std::shared_ptr<Node>& node, FormatContext& ctx) const
    {
        // Get the string representation of the node
        std::string nodeRepresentation = node ? node->getName() : "null";

        // Format and return the result
        return std::format_to(ctx.out(), "Node: {}", nodeRepresentation);
    }
};


class NodeProperty
{
public:
	public:
	NodeProperty() = default;
	NodeProperty(std::shared_ptr<Node> node) : m_node(node){}


  	std::shared_ptr<Node> m_node;
};

class Edge
{
};

class EdgeProperty
{
};

/**
 * @brief
 *
 */
class NodeGraph
{

  public:
	template <typename NodeType>
	void addNode(NodeType&& node)
	{
		auto sharedNode = std::make_shared<std::decay_t<NodeType>>(std::forward<NodeType>(node));

        m_nodes.push_back(id);
        m_nodeProperties[id].m_node = sharedNode;
        
        id++;
	}




	void removeNode();
	void addEdge();
	void removeEdge();

	std::vector<uint64_t> getNodes() const
	{
		return m_nodes;
	}

	std::vector<uint64_t> getEdges() const
	{
		return m_edges;
	}

	NodeProperty getNodeProperty(uint64_t id) const
	{
		return m_nodeProperties.at(id);
	}


  private:



	static uint64_t id;

	std::vector<uint64_t> m_nodes;
	std::vector<uint64_t> m_edges;

	std::map<uint64_t, NodeProperty> m_nodeProperties;
	std::map<uint64_t, std::vector<EdgeProperty>> m_edgeProperties;
};

uint64_t NodeGraph::id = 0;


int main()
{
	/*
	std::cout << "Hello World!" << std::endl;

	NodeGraph graph;

	std::shared_ptr<AddNode> nodeAdd = graph.addNode(std::make_shared<AddNode>());
	nodeAdd->m_inputs.input1.setDefaultValue(10u);
	nodeAdd->m_inputs.input2.setDefaultValue(20u);
	nodeAdd->compute();
	std::cout << "Output from Node 1: " << nodeAdd->m_outputs.output1() << std::endl;


	std::shared_ptr<MultiplyNode> nodeMultiply = graph.addNode(std::make_shared<MultiplyNode>());
	nodeMultiply->m_inputs.input1.setDefaultValue(10u);
	nodeMultiply->m_inputs.input2.setDefaultValue(20u);
	nodeMultiply->compute();

	std::cout << "Output from Node 2: " << nodeMultiply->m_outputs.output1() << std::endl;

	//TODO better way to select which output to connect to which input
	graph.connectNodes(nodeAdd, nodeAdd->getOutputHandles().at(0), nodeMultiply, nodeMultiply->getInputHandles().at(0));

	nodeMultiply->compute();
	std::cout << "Output from Node 2 after connection: " << nodeMultiply->m_outputs.output1() << std::endl;



	//Display the graph
	std::cout << "--- Graph ---" << std::endl;
	for(auto& node : graph.getNodes())
	{
		std::cout << "\tNode: " << node->getName() << std::endl;
		std::cout << "\t\tInputs: " << std::endl;
		for(const auto& inputHandle : node->getInputHandles())
		{
			std::cout << "\t\t\t" << inputHandle.getName() << std::endl;
			std::cout << "\t\t\t\tType: " << inputHandle.getTypeInfo().getBaseTypeString() << std::endl;
			std::cout << "\t\t\t\tComponent Count: " << inputHandle.getTypeInfo().getComponentCountValue() << std::endl;
			std::cout << "\t\t\t\tValue: " << inputHandle.getData<uint32_t>() << std::endl;
			//std::cout << "\t\t\t\tDefault Value: " << inputHandle.getData<uint32_t>() << std::endl;
		}

		std::cout << "\t\tOutputs: " << std::endl;
		for(const auto& outputHandle: node->getOutputHandles())
		{
			std::cout << "\t\t\t" << outputHandle.getName() << std::endl;
			std::cout << "\t\t\t\tType: " << outputHandle.getTypeInfo().getBaseTypeString() << std::endl;
			std::cout << "\t\t\t\tComponent Count: " << outputHandle.getTypeInfo().getComponentCountValue() << std::endl;
			std::cout << "\t\t\t\tValue: " << outputHandle.getData<uint32_t>() << std::endl;
		}

	}

	//Connections
	std::cout << "--- Connections ---" << std::endl;
	for(const auto& connection : graph.getConnections())
	{
		std::cout << "Connection: " << std::endl;
		std::cout << "\tFrom: " << connection.m_nodeConnectedFrom.lock()->getName() << std::endl;
		std::cout << "\tOutput: " << connection.m_outputHandle.getName() << std::endl;
		std::cout << "\tTo: " << connection.m_nodeConnectedTo.lock()->getName() << std::endl;
		std::cout << "\tInput: " << connection.m_inputHandle.getName() << std::endl;
	}
	*/
	NodeGraph nodeGraph;

	NodeMultiInput node;

	nodeGraph.addNode(node);

    /*-----------------*/
	/*Display the graph*/
	/*-----------------*/
	std::print("Graph:\n");
	for (const auto& node : nodeGraph.getNodes())
	{
		std::shared_ptr<Node> nodePtr = nodeGraph.getNodeProperty(node).m_node;
		std::print("\tNode: {}\n", nodePtr);
	}

	return 0;
}