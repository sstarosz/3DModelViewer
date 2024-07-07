#include "Nodes/Node.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace st::core::test
{
    
    class NodeGraphTest : public ::testing::Test
    {
        public:

        class TestableNode : public Node
        {
            public:
            TestableNode() = default;
            ~TestableNode() = default;

            bool initialize() override
            {
                // Do nothing
                return true;
            }

            bool compute() override
            {
                // Do nothing
                return true;
            }

        };



    };





    TEST_F(NodeGraphTest, NodeGraphConstructor)
    {
        NodeGraph nodeGraph;
        ASSERT_TRUE(true);
    }



    TEST_F(NodeGraphTest, NodeGraphAddNode)
    {
        NodeGraph nodeGraph;



        std::shared_ptr<Node> testNodePtr = std::make_shared<TestableNode>();
        std::shared_ptr<Node> testNodePtr1 = std::make_shared<TestableNode>();
        std::shared_ptr<Node> testNodePtr2 = std::make_shared<TestableNode>();
        std::shared_ptr<Node> testNodePtr3 = std::make_shared<TestableNode>();



        nodeGraph.addNode(std::move(testNodePtr));
        nodeGraph.addNode(std::move(testNodePtr1));
        nodeGraph.addNode(std::move(testNodePtr2));
        nodeGraph.addNode(std::move(testNodePtr3));
        ASSERT_TRUE(true);
    }

}