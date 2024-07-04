


Things that could be improved:

- You use m_inputs.meshData = core::TypedAttribute<core::MeshData>::Builder("Mesh Data").build();

to create the attribute but the definition of input use 
core::TypedInputHandler<core::MeshData>

which is not obvious. It would be better to use the same name for both.(?)