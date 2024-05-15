#include <iostream>

enum class BaseType
{
    eUnknown = 0,
    eBool,
    eInt,
    eUInt,
    eFloat,
    eDouble,
    eTypeCount
};

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

enum class ArrayDimension
{
    eUnknown = 0,
    e1D,
    e2D,
    e3D,
    eTypeCount
};

/**
 * @brief Class that contains runtime type information about type
 */
class Type
{
  public:
	Type() = default;
	Type(BaseType baseType, ComponentCount componentCount, ArrayDimension arrayDimension) :
		m_baseType(baseType),
		m_componentCount(componentCount),
		m_arrayDimension(arrayDimension)
	{
	}

  private:
	BaseType m_baseType = BaseType::eUnknown;
	ComponentCount m_componentCount{ComponentCount::eScalar};
	ArrayDimension m_arrayDimension{ArrayDimension::e1D};
};

class Attribute
{
  public:
    Attribute() = default;
    Attribute(const std::string& name, const Type& type) :
        m_name(name),
        m_type(type)
    {
    }

  private:
    std::string m_name;
    Type m_type;
};


class Plug
{

    private:
    Type m_type;
};


template <typename InputTypeT>
class InputBase
{
    using InputType = InputTypeT;
public:

    const InputType& operator()() const
    {
        return m_value;
    }

    InputType& operator=(const InputType& value)
    {
        m_value = value;
        return m_value;
    }

    InputType& operator+=(const InputType& value)
    {
        m_value += value;
        return m_value;
    }

private:
    InputType m_value;
};


class Node
{
public:

    struct inputs
    {
        InputBase<uint32_t> input1;
        InputBase<uint32_t> input2;
    };

    struct outputs
    {
        uint32_t output1;
    };

    void compute(inputs& inputs, outputs& outputs)
    {
        // Compute the output
        auto result = inputs.input1() + inputs.input2();

        // Set the output
        outputs.output1 = result;
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
    //static std::vector<InputsAtributes> m_inputs;
    //static std::vector<OutputAtributes> m_outputs;
};


class NodeGraph
{
public:
    NodeGraph() = default;
    ~NodeGraph() = default;

};



int main()
{
    std::cout << "Hello World!" << std::endl;

    Node node;
    node.m_inputs = 10u;
    node.m_inputs = 20u;
    node.compute(node.m_inputs, node.m_outputs);

    std::cout << "Output: " << node.m_outputs.output1 << std::endl;

    return 0;
}