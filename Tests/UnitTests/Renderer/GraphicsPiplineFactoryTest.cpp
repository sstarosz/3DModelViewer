
#include "GraphicsPiplineFactory.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <array>
#include <fstream>
#include <string>

// PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXTTest;
// PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXTTest;
//
//// TODO - change it to class method
//[[maybe_unused]] VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
//																			   const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
//																			   const VkAllocationCallbacks* pAllocator,
//																			   VkDebugUtilsMessengerEXT* pMessenger)
//{
//	return pfnVkCreateDebugUtilsMessengerEXTTest(instance, pCreateInfo, pAllocator, pMessenger);
//}
//
//[[maybe_unused]] VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
//																			VkDebugUtilsMessengerEXT messenger,
//																			VkAllocationCallbacks const* pAllocator)
//{
//	return pfnVkDestroyDebugUtilsMessengerEXTTest(instance, messenger, pAllocator);
//}

namespace st::renderer::test
{
	VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
						   VkDebugUtilsMessageTypeFlagsEXT messageType,
						   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
						   void*)
	{
		std::ostringstream message;

		message << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": "
				<< vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageType)) << ":\n";
		message << "\t"
				<< "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
		message << "\t"
				<< "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
		message << "\t"
				<< "message         = <" << pCallbackData->pMessage << ">\n";

		if (0 < pCallbackData->queueLabelCount)
		{
			message << "\t"
					<< "Queue Labels:\n";
			for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++)
			{
				message << "\t\t"
						<< "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
			}
		}

		if (0 < pCallbackData->cmdBufLabelCount)
		{
			message << "\t"
					<< "CommandBuffer Labels:\n";
			for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
			{
				message << "\t\t"
						<< "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
			}
		}

		if (0 < pCallbackData->objectCount)
		{
			message << "\t"
					<< "Objects:\n";
			for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
			{
				message << "\t\t"
						<< "Object " << i << "\n";
				message << "\t\t\t"
						<< "objectType   = " << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) << "\n";
				message << "\t\t\t"
						<< "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
				if (pCallbackData->pObjects[i].pObjectName)
				{
					message << "\t\t\t"
							<< "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
				}
			}
		}

		std::cout << message.str() << std::endl;

		return false;
	}

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

			std::vector<const char*> extensions = {
				VK_KHR_SURFACE_EXTENSION_NAME,
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

			std::vector<const char*> layers = {
				"VK_LAYER_KHRONOS_validation",
			};

			vk::InstanceCreateInfo createInfo({}, &appInfo, layers, extensions);

			m_instance = vk::createInstance(createInfo);

			// Setup debug messenger
			auto pfnVkCreateDebugUtilsMessengerEXTTest = m_instance.getProcAddr("vkCreateDebugUtilsMessengerEXT");
			if (!pfnVkCreateDebugUtilsMessengerEXTTest)
			{
				// TODO - Change it to something independent of iostream
				std::cout << "GetInstanceProcAddr: Unable to find "
							 "pfnVkCreateDebugUtilsMessengerEXT function."
						  << std::endl;
				exit(1);
			}

			auto pfnVkDestroyDebugUtilsMessengerEXTTest = m_instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT");
			if (!pfnVkDestroyDebugUtilsMessengerEXTTest)
			{
				// TODO - Change it to something independent of iostream
				std::cout << "GetInstanceProcAddr: Unable to find "
							 "pfnVkDestroyDebugUtilsMessengerEXT function."
						  << std::endl;
				exit(1);
			}

			vk::DebugUtilsMessageSeverityFlagsEXT severityFlags{
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eError};

			vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags{
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
				vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance};

			vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
				{},
				severityFlags,
				messageTypeFlags,
				&debugCallback};

			m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);

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

	class TestableRenderPass
	{
	  public:
		TestableRenderPass(TestableVulkanContext& context) :
			m_context(context)
		{
		}

		void initialize()
		{
			vk::AttachmentDescription colorAttachment({},
													  vk::Format::eB8G8R8A8Unorm,
													  vk::SampleCountFlagBits::e1,
													  vk::AttachmentLoadOp::eClear,
													  vk::AttachmentStoreOp::eStore,
													  vk::AttachmentLoadOp::eDontCare,
													  vk::AttachmentStoreOp::eDontCare,
													  vk::ImageLayout::eUndefined,
													  vk::ImageLayout::ePresentSrcKHR);

			vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

			vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef, nullptr, nullptr, 0, nullptr);

			vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL,
											 0,
											 vk::PipelineStageFlagBits::eColorAttachmentOutput,
											 vk::PipelineStageFlagBits::eColorAttachmentOutput,
											 {},
											 vk::AccessFlagBits::eColorAttachmentRead |
												 vk::AccessFlagBits::eColorAttachmentWrite);

			vk::RenderPassCreateInfo renderPassInfo({}, 1, &colorAttachment, 1, &subpass, 1, &dependency);

			m_renderPass = m_context.m_device.createRenderPass(renderPassInfo);
		}

	  public:
		TestableVulkanContext& m_context;
		vk::RenderPass m_renderPass;
	};


	TEST(GraphicsPiplineFactoryTest, CreateGraphicsPipeline)
	{
		TestableVulkanContext context;
		context.initialize();

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

		Pipeline pipeline = PipelineBuilder(context)
								.setVertexShader(vertexShader)
								.setFragmentShader(fragmentShader)
								.build();

		// TODO
		// st::renderer::GraphicsPiplineFactory factory;
		// st::renderer::GraphicsPipeline pipeline = factory.createGraphicsPipeline();
		// EXPECT_TRUE(pipeline.isValid());
	}

} // namespace st::renderer::test