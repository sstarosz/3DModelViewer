
#include "GraphicsPiplineFactory.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <array>
#include <fstream>
#include <string>

namespace st::renderer::test
{

	class TestableVulkanContext : public VulkanContext
	{
	  public:
		TestableVulkanContext()
		{
			m_instance = vk::Instance{};
			m_physicalDevice = vk::PhysicalDevice{};
			m_device = vk::Device{};
		}

		void initialize()
		{
			vk::ApplicationInfo appInfo("Test Instance", VK_MAKE_VERSION(1, 0, 0), "No Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_2);

			vk::InstanceCreateInfo createInfo({}, &appInfo);

			m_instance = vk::createInstance(createInfo);

			// Chose physical device
			std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();
			m_physicalDevice = devices.front();

			// Specify queue families
			float queuePriority = 1.0f;
			vk::DeviceQueueCreateInfo queueCreateInfo({}, 0, 1, &queuePriority);

			// Specify device features
			vk::PhysicalDeviceFeatures deviceFeatures;

			// Create logical device
			vk::DeviceCreateInfo logicalDeviceCreateInfo({}, 1, &queueCreateInfo, 0, nullptr, 0, nullptr, &deviceFeatures);
			m_device = m_physicalDevice.createDevice(logicalDeviceCreateInfo);
		}
	};

	class TestPipeline : public PipelineBuilder
	{
	  public:
		TestPipeline(const VulkanContext& context, const vk::RenderPass& renderPass) :
			PipelineBuilder(context, renderPass)
		{
		}

		void buildShader() override
		{
			// TODO
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

			// Compile shaders
			pipeline.shaderStages = compileShader(vertexShader, fragmentShader);
		};

		void buildDynamicState() override
		{
			// TOD
			std::array dynamicStates{vk::DynamicState::eViewport, vk::DynamicState::eScissor};
			pipeline.dynamicStateInfo = vk::PipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags{},
																		   dynamicStates);
		};
	};

	TEST(GraphicsPiplineFactoryTest, CreateGraphicsPipeline)
	{
		TestableVulkanContext context;
		context.initialize();
		vk::RenderPass renderPass;

		TestPipeline pipeline{context, renderPass};

		PipelineDirector director;
		director.setBuilder(&pipeline);

		director.constructPipeline();

		Pipeline graphicsPipeline = director.getPipeline();

		// TODO
		// st::renderer::GraphicsPiplineFactory factory;
		// st::renderer::GraphicsPipeline pipeline = factory.createGraphicsPipeline();
		// EXPECT_TRUE(pipeline.isValid());
	}

} // namespace st::renderer::test