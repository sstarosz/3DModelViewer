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

		AttributeBase(Type type) :
			m_type(type)
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

	template<typename T>
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
			m_name(name)
		{
		}


        private:
        T m_value;
    };

    template<typename T>
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

    template<typename T>
    class Input : public Attribute<T>
    {
        public:
        Input() : Attribute<T>()
        {
            this->setWritable(true);
            this->setReadable(false);
        }

        Input(T value) :
            Attribute<T>(value)
        {
        }

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


    protected:  //TODO refactor to private?
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

        template<typename T>
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

        template<typename T>
        const T& getOutput() const
        {
            Output<T>* output = dynamic_cast<Output<T>*>(m_attribute);
            return *output;
        }
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
            m_attributes.push_back(Handler(&input)); //TODO refactor
        }

        template<typename T>
        void addOutput(Output<T>& output)
        {
            m_attributes.push_back(Handler(&output)); //TODO refactor
        }

        virtual bool initialize() = 0;
        virtual bool compute() = 0;


        std::string getName() const
        {
            return m_name;
        }

        std::vector<Handler> getAttributes() const
        {
            return m_attributes;
        }

        private:
        std::string m_name;

        std::vector<Handler> m_attributes;
    };


    class Connection
    {

    };

    class NodeGraph
    {


        std::vector<std::shared_ptr<Node2>> m_nodes;

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