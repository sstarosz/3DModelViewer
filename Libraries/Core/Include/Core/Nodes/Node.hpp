#ifndef ST_CORE_NODES_NODE_HPP
#define ST_CORE_NODES_NODE_HPP

#include <string>
#include <variant>
#include <vector>
#include <optional>
#include <cassert>

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

    enum class Type
    {
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

    //Function that convert Type to enum
    template<typename T>
    constexpr Type convertTypeToEnum()
    {
        if(std::is_same<T, int32_t>::value)
        {
            return Type::Int;
        }
        else if(std::is_same<T, uint32_t>::value)
        {
            return Type::UInt;
        }
        else if(std::is_same<T, float>::value)
        {
            return Type::Float;
        }
        else if(std::is_same<T, double>::value)
        {
            return Type::Double;
        }
        else if(std::is_same<T, bool>::value)
        {
            return Type::Bool;
        }
        else if(std::is_same<T, MeshData>::value)
        {
            return Type::MeshData;
        }
        //else if(std::is_same<T, Eigen::Vector2f>::value)
        //{
        //    return Type::Vector2f;
        //}
        //else if(std::is_same<T, Eigen::Vector3f>::value)
        //{
        //    return Type::Vector3f;
        //}
        //else if(std::is_same<T, Eigen::Vector4f>::value)
        //{
        //    return Type::Vector4f;
        //}
        //else if(std::is_same<T, Eigen::Matrix2f>::value)
        //{
        //    return Type::Matrix2f;
        //}
        //else if(std::is_same<T, Eigen::Matrix3f>::value)
        //{
        //    return Type::Matrix3f;
        //}
        //else if(std::is_same<T, Eigen::Matrix4f>::value)
        //{
        //    return Type::Matrix4f;
        //}
        else
        {
            assert(false && "Type not supported");
        }
    }


    class AttributeBase
    {
        public:
        AttributeBase() = default;
        AttributeBase(Type type) : m_type(type)
        {
        }
        virtual ~AttributeBase() = default;

        Type getType() const
        {
            return m_type;
        }

        private:
        Type m_type;
    };


    template<typename T>
    class Attribute : public AttributeBase
    {
	  public:
		Attribute() :
			AttributeBase(convertTypeToEnum<T>()),
			m_value(T()),
			m_name("")
		{
		}

		Attribute(T value) :
			AttributeBase(convertTypeToEnum<T>()),
			m_value(value),
			m_name("")
		{
		}

		Attribute(const std::string& name) :
			m_name(name)
		{
		}

		void setName(const std::string& name)
        {
            m_name = name;
        }

        void setDefault(const T& value)
        {
            m_value = value;
        }

        void setMin(const T& value)
        {
            m_minValue = value;
        }

        void setMax(const T& value)
        {
            m_maxValue = value;
        }

        void setSoftMinValue(const T& value)
        {
            m_softMinValue = value;
        }

        void setSoftMaxValue(const T& value)
        {
            m_softMaxValue = value;
        }


        const std::string& getName() const
        {
            return m_name;
        }

        private:
        T m_value;

        std::string m_name;
        std::optional<T> m_defaultValue;
        std::optional<T> m_minValue;
        std::optional<T> m_maxValue;
        std::optional<T> m_softMinValue;
        std::optional<T> m_softMaxValue;
    };



    template<typename T>
    class Output : public Attribute<T>
    {
        public:
        Output() : 
            Attribute<T>(T())
        {
        }

        Output(T value) :
            Attribute<T>(value)
        {
        }

    };

    template<typename T>
    class Input : public Attribute<T>
    {
        public:
        Input() : Attribute<T>()
        {
        }

        Input(T value) :
            Attribute<T>(value)
        {
        }

    };



    class Handler
    {
        public:
        Handler() = default;
        virtual ~Handler() = default;
    };

    class InputHandler : public Handler
    {
        public:
        template <typename T>
        InputHandler(Input<T>* input) :
         Handler(),
         m_input(input)
        {
        }

        template<typename T>
        const T& getInput() const
        {
            Input<T>* input = dynamic_cast<Input<T>*>(m_input);
            return *input;
        }

        std::string getName() const
        {
            Type m_type = m_input->getType();

            //Cast m_input to the correct type
            if(m_type == Type::UInt)
            {
                Input<uint32_t>* input = dynamic_cast<Input<uint32_t>*>(m_input);
                return input->getName();
            }
            else if(m_type == Type::Float)
            {
                Input<float>* input = dynamic_cast<Input<float>*>(m_input);
                return input->getName();
            }
            else
            {
                assert(false && "Type not supported");
            }
        }


        private:
        AttributeBase* m_input;
    };

    class OutputHandler : public Handler
    {
        public:
        template <typename T>
        OutputHandler(Output<T>* output) :
         Handler(),
         m_output(output)
        {
        }

        template<typename T>
        const T& getOutput() const
        {
            Output<T>* output = dynamic_cast<Output<T>*>(m_output);
            return *output;
        }

        std::string getName() const
        {
            //TODO overcomplicated
            Type m_type = m_output->getType();

            //Cast m_input to the correct type
            if(m_type == Type::UInt)
            {
                Output<uint32_t>* output = dynamic_cast<Output<uint32_t>*>(m_output);
                return output->getName();
            }
            else if(m_type == Type::Float)
            {
                Output<float>* output = dynamic_cast<Output<float>*>(m_output);
                return output->getName();
            }
            else if(m_type == Type::MeshData)
            {
                Output<MeshData>* output = dynamic_cast<Output<MeshData>*>(m_output);
                return output->getName();
            }
            else
            {
                assert(false && "Type not supported");
            }
        }

        private:
        AttributeBase* m_output;
    };



    class Node2
    {
    public:
        virtual ~Node2() = default;

        void defineNode(const std::string& name)
        {
            m_name = name;
        }

        template<typename T>
        void addInput(Input<T>& input)
        {
            m_inputHandlers.push_back(InputHandler(&input));
        }

        template<typename T>
        void addOutput(Output<T>& output)
        {
            m_outputHandlers.push_back(OutputHandler(&output));
        }

        virtual bool initialize() = 0;
        virtual bool compute() = 0;


        std::string getName() const
        {
            return m_name;
        }

        std::vector<InputHandler> getInputHandlers() const
        {
            return m_inputHandlers;
        }

        std::vector<OutputHandler> getOutputHandlers() const
        {
            return m_outputHandlers;
        }

        private:
        std::string m_name;

        std::vector<InputHandler> m_inputHandlers;
        std::vector<OutputHandler> m_outputHandlers;
    };


    template<typename NodeType>
    class RegisterNode
    {
        public:
        RegisterNode()
        {
            static_assert(std::is_base_of<Node2, NodeType>::value, "NodeType must derive from Node2");

        }

        //NodeType* getNode() const
        //{
        //    return m_node;
        //}
//
        //private:
        //std::map<std::string, NodeType*> m_nodes;
    };

} // namespace st::core

#endif // ST_CORE_NODES_NODE_HPP