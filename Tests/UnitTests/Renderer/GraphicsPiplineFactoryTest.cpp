
#include "GraphicsPiplineFactory.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <Eigen/Core>

namespace st::renderer::test
{


class TestPipeline : public PipelineBuilder
{
    public:

    void buildShader() override
    {
        //TODO
        std::string vertexShader = R"(
            #version 450

            layout(location = 0) in vec3 inPosition;
            layout(location = 1) in vec3 inColor;

            layout(location = 0) out vec3 outFragColor;

            layout(set = 0, binding = 0) uniform MVP
            {
                mat4 model;
                mat4 view;
                mat4 proj;
            } mvp;

            void main()
            {
                gl_Position = vec4(inPosition, 1.0);
                outFragColor = inColor;
            }
        )";

        std::string fragmentShader = R"(
            #version 450

            layout(location = 0) in vec3 inFragColor;

            layout(location = 0) out vec4 outColor;

            void main()
            {
                outColor = vec4(inFragColor, 1.0);
            }
        )";

        //Compile shaders
        shaderStages = compileShader(vertexShader, fragmentShader);
        
        registerUniformBuffer("MVP",
                               vk::DescriptorType::eUniformBuffer,
                               sizeof(Eigen::Matrix4f) * 3,
                               0,
                               0,
                               vk::ShaderStageFlagBits::eVertex);

        createDescriptorSetLayout();
        initializeUniformBuffers();

    };

    void buildVertexInput() override
    {
        //TODO
        vk::VertexInputBindingDescription vertexInputBinding = vk::VertexInputBindingDescription(0, sizeof(float) * 6, vk::VertexInputRate::eVertex);

        std::array<vk::VertexInputAttributeDescription, 4> vertexInputAttributes
        {
            vk::VertexInputAttributeDescription{0, 0, vk::Format::eR32G32B32Sfloat, 0},
            vk::VertexInputAttributeDescription{1, 0, vk::Format::eR32G32B32Sfloat, sizeof(float) * 3}
        };


        vertexInputInfo = vk::PipelineVertexInputStateCreateInfo{{}, 
                                                               vertexInputBinding, 
                                                               vertexInputAttributes};

        inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{{}, 
                                                                vk::PrimitiveTopology::eTriangleList, 
                                                                VK_FALSE};
    };


    void buildViewport() override
    {
        //TODO
        viewport = vk::Viewport(0.0f, 0.0f, 800.0f, 600.0f, 0.0f, 1.0f);
        scissor = vk::Rect2D({0, 0}, {800, 600});
        viewportState = vk::PipelineViewportStateCreateInfo{{}, 1, &viewport, 1, &scissor};
    };




};





TEST(GraphicsPiplineFactoryTest, CreateGraphicsPipeline)
{
    //TODO
    //st::renderer::GraphicsPiplineFactory factory;
    //st::renderer::GraphicsPipeline pipeline = factory.createGraphicsPipeline();
    //EXPECT_TRUE(pipeline.isValid());
}



} // namespace st::renderer::test