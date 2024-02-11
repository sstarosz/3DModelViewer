#ifndef ST_RENDERER_VISUAL_AIDS_PIPELINE_HPP
#define ST_RENDERER_VISUAL_AIDS_PIPELINE_HPP


#include "GraphicsPiplineFactory.hpp"

namespace st::renderer
{

class VisualAidsPipeline : public PipelineBuilder
{
public:
    VisualAidsPipeline(const VulkanContext& context, const vk::RenderPass& renderPass):
    PipelineBuilder(context, renderPass)
    {

    }

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
        pipeline.shaderStages = compileShader(vertexShader, fragmentShader);
    }

    void buildDynamicState() override
    {
        pipeline.dynamicStates.push_back(vk::DynamicState::eViewport);
        pipeline.dynamicStates.push_back(vk::DynamicState::eScissor);
        pipeline.dynamicStateInfo = vk::PipelineDynamicStateCreateInfo{vk::PipelineDynamicStateCreateFlags{}, 
                                                                       pipeline.dynamicStates};
    }


};




} // namespace st::renderer

#endif // ST_RENDERER_VISUAL_AIDS_PIPELINE_HPP