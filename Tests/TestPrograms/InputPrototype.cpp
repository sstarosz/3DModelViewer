

//Design class Input that can store data of different type.
//This class will be use to  represent input to a node.
//Each node store Input class of different type InputTypes and execute operation on it.
//Like reading float, int or string values from input.

//Because Input class is template class, it can store data of any type, there will be
//needed to create InputHandle class that will store pointer to Input class and provide
//common interface to access input data and query information about input.
#include <iostream>
#include <string>
#include <memory>
#include <cassert>

// Design class Input that can store data of different types.
class InputBase 
{
public:
    virtual ~InputBase() {}
};

template <typename T>
class Input : public InputBase
{
public:
    using InputType = T;
public:
    Input(const InputType& data) : m_data(data) {}

    InputType getData() const {
        return m_data;
    }

private:
    InputType m_data;
};

// Create InputHandle class that will store a pointer to Input class and provide
// a common interface to access input data and query information about input.
class InputHandle {
public:
    template <typename T>
    InputHandle(Input<T>* input) : 
    m_input(input)
    {
        assert(input != nullptr && "Input is nullptr");
    }

    template <typename T>
    T getData() const 
    {
        Input<T>* input = dynamic_cast<Input<T>*>(m_input);
        if (input == nullptr) 
        {
            throw std::runtime_error("Invalid type");
        }

        return input->getData();
    }

private:
    InputBase* m_input;
};

int main() {
    // Example usage
    Input<int> intInput(42);
    InputHandle intHandle(&intInput);
    std::cout << intHandle.getData<int>() << std::endl;

    Input<std::string> stringInput("Hello, world!");
    InputHandle stringHandle(&stringInput);
    std::cout << stringHandle.getData<std::string>() << std::endl;

    return 0;
}

