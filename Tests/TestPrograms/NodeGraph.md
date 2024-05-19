# Node Graph Requirement


Main Graph is a main application graph that contains multiple sub-graphs and individual nodes. Each sub-graph can contain multiple nodes.



## Graph Requirements
* Main Graph can contain multiple sub-graphs and individual nodes.
* Sub-Graphs expose their input and output ports to the parent graph.
* Main Graph also contain main application nodes like RendererNode, InputNode, etc.

* Sub-Graphs can be connected to other sub-graphs or individual nodes.

* Graph maintains a list of all nodes and sub-graphs. But not the nodes inside the sub-graphs.

* Graph is responsible for creating and destroying nodes and sub-graphs.

* Graph is responsible for connecting and disconnecting nodes and sub-graphs.


## Sub-Graph Requirements


## Node Requirements

## Node Input Requirements

## Node Output Requirements

## Node Connection Requirements



## Example

```
Main Graph
|
|--- Sub-Graph 1
|    |
|    |--- Node A
|    |    |
|    |    |--- Inputs
|    |    |    |
|    |    |    |--- input1
|    |    |    |--- input2
|    |    |
|    |    |--- Outputs
|    |         |
|    |         |--- output1
|    |
|    |--- Node B
|         |
|         |--- Inputs
|         |    |
|         |    |--- input1 (connected to output1 of Node A)
|         |    |--- input2
|         |
|         |--- Outputs
|              |
|              |--- output1
|
|--- Sub-Graph 2
     |
     |--- Node C
          |
          |--- Inputs
          |    |
          |    |--- input1 (connected to output1 of Node B in Sub-Graph 1)
          |    |--- input2
          |
          |--- Outputs
               |
               |--- output1


```