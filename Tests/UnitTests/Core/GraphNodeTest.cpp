#include "NodeGraph.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::core::test
{
    
    class NodeGraphTest : public ::testing::Test
    {

    };


    TEST_F(NodeGraphTest, CameraConstructor)
    {
        NodeGraph nodeGraph;

        Node testNode(Node::Type::eUnknown);
        Node testNode1(Node::Type::eCamera);
        Node testNode2(Node::Type::eLight);
        Node testNode3(Node::Type::eMaterial);



        nodeGraph.addNode(std::move(testNode));
        nodeGraph.addNode(std::move(testNode1));
        nodeGraph.addNode(std::move(testNode2));
        nodeGraph.addNode(std::move(testNode3));
        

        nodeGraph.dumpNodeGraph();
    }

}