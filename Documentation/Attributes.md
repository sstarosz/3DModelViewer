


Things that could be improved:

- You use m_inputs.meshData = core::TypedAttribute<core::MeshData>::Builder("Mesh Data").build();

to create the attribute but the definition of input use 
core::TypedInputHandler<core::MeshData>

which is not obvious. It would be better to use the same name for both.(?)



it not clear that in derivered builder you need to make build function which is not virtual and becouse of that it is easy to forget to implement it.


Since constructor of attribute is protected, it is not possible to use make_shared to create it.
```auto attribute = std::shared_ptr<TestAttribute>(new TestAttribute());```