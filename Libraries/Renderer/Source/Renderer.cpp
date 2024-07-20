#include "Renderer.hpp"
#include "Core/EventSystem.hpp"
#include "MaterialManager.hpp"
#include "PhysicalDevice.hpp"
#include "QueueFamily.hpp"
#include "StandardMaterial.hpp"
#include "VulkanContext.hpp"

#include <Eigen/Core>
#include <vulkan/vulkan.hpp>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

// TODO - change it to class method
[[maybe_unused]] VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
																			   const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
																			   const VkAllocationCallbacks* pAllocator,
																			   VkDebugUtilsMessengerEXT* pMessenger)
{
	return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

[[maybe_unused]] VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
																			VkDebugUtilsMessengerEXT messenger,
																			VkAllocationCallbacks const* pAllocator)
{
	return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

namespace st::renderer
{
	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
	{
		SwapChainSupportDetails details;

		details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
		details.formats = device.getSurfaceFormatsKHR(surface);
		details.presentModes = device.getSurfacePresentModesKHR(surface);

		return details;
	}

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

	class Renderer::PrivateRenderer
	{
	  public:
		PrivateRenderer() :
			m_vulkanContext(),
			m_physicalDevice(m_vulkanContext)
		{
			spdlog::info("Renderer::PrivateRenderer()");
		}

		~PrivateRenderer()
		{
		}

		void createInstance()
		{
			vk::ApplicationInfo appInfo{
				"St Model Viewer",
				VK_MAKE_VERSION(1, 0, 0),
				"St Model Viewer",
				VK_MAKE_VERSION(1, 0, 0),
				VK_API_VERSION_1_3};

			std::vector<const char*> extensions = {
				VK_KHR_SURFACE_EXTENSION_NAME,
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

			std::vector<const char*> layers = {
				"VK_LAYER_KHRONOS_validation",
			};

			vk::InstanceCreateInfo createInfo{
				{},
				&appInfo,
				layers,
				extensions};

			m_vulkanContext.m_instance = vk::createInstance(createInfo);
		}

		vk::Instance getVulkanInstance() const
		{
			return m_vulkanContext.m_instance;
		}

		void init()
		{
			//Create not graphics objects
			createInstance();
			createDebugMessageUtils();
			m_physicalDevice.initialize();
			createLogicalDevice();

		}

		void initSurface(vk::SurfaceKHR surface)
		{
			//Create graphics objects
			m_vulkanContext.m_surface = surface;

			////Check if the physical device supports the surface
			// if (!m_physicalDevice.checkQueueFamiliesSupport(m_vulkanContext.m_physicalDevice, m_vulkanContext.m_surface))
			//{
			//	throw std::runtime_error("Physical device does not support the surface!");
			// }

			createSwapChain();

			//Pick the first available queue family
			QueueFamily indices = QueueFamily::findQueueFamilies(m_vulkanContext.m_physicalDevice, m_vulkanContext.m_surface);
			m_vulkanContext.m_graphicsQueue = m_vulkanContext.m_device.getQueue(indices.m_graphicsFamily.value(), 0);
			m_vulkanContext.m_presentQueue = m_vulkanContext.m_device.getQueue(indices.m_presentFamily.value(), 0);


			createRenderPass();

			updateScene(m_input);
			updateVertexBuffer();
			updateIndexBuffer();

			createCommandPool();
			createFrameBuffer();
			createCommandBuffers();
			createSyncObjects();

			m_initialized = true;


		}

		void waitForPreviousFrame()
		{
			auto resultFence = m_vulkanContext.m_device.waitForFences(m_vulkanContext.m_inFlightFences.at(m_currentFrame),
																	  VK_TRUE,
																	  UINT64_MAX);
			if (resultFence != vk::Result::eSuccess)
			{
				throw std::runtime_error("Failed to wait for fences!");
			}

			// TODO - Check for other results:
			// vk::Result::eTimeout
			// vk::Result::eErrorDeviceLost
			// vk::Result::eErrorOutOfDeviceMemory
		}

		uint32_t acquireSwapChainImage()
		{
			auto [result, imageIndex] = m_vulkanContext.m_device.acquireNextImageKHR(m_vulkanContext.m_swapchain,
																					 UINT64_MAX,
																					 m_vulkanContext.m_imageAvailableSemaphores[m_currentFrame],
																					 VK_NULL_HANDLE);
			if (result == vk::Result::eErrorOutOfDateKHR)
			{
				recreateSwapChain();
				return acquireSwapChainImage();
			}
			else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
			{
				throw std::runtime_error("Failed to acquire swap chain image!");
			}

			// TODO - Check for other results:
			// vk::Result::eTimeout
			// vk::Result::eNotReady
			// vk::Result::eSuboptimalKHR

			// vk::Result::eErrorOutOfHostMemory
			// vk::Result::eErrorOutOfDeviceMemory
			// vk::Result::eErrorDeviceLost
			// vk::Result::eErrorOutOfDateKHR
			// vk::Result::eErrorSurfaceLostKHR
			// vk::Result::eErrorFullScreenExclusiveModeLostEXT

			return imageIndex;
		}

		void resetCommandBuffer(uint32_t currentFrame)
		{
			m_vulkanContext.m_commandBuffers[currentFrame].reset(vk::CommandBufferResetFlagBits::eReleaseResources);
		}

		void render()
		{
			// Wait for previous frame to finish
			waitForPreviousFrame();

			// Reset the fence for the current frame
			m_vulkanContext.m_device.resetFences(m_vulkanContext.m_inFlightFences.at(m_currentFrame));

			// Acquire swap chain image
			uint32_t imageIndex = acquireSwapChainImage();

			// Reset the command buffer for the current frame
			resetCommandBuffer(m_currentFrame);

			// Record command for the current frame
			recordCommandBuffer(m_vulkanContext.m_commandBuffers[m_currentFrame], imageIndex);

			// Submit command buffer
			submitCommandBuffer(m_currentFrame);

			// Present image
			presentImage(imageIndex);

			// Advance to next frame
			m_currentFrame = (m_currentFrame + 1) % m_vulkanContext.MAX_FRAMES_IN_FLIGHT;
		}

		void submitCommandBuffer(uint32_t currentFrame)
		{
			vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

			vk::SubmitInfo submitInfo(m_vulkanContext.m_imageAvailableSemaphores[currentFrame],
									  waitDestinationStageMask,
									  m_vulkanContext.m_commandBuffers[currentFrame],
									  m_vulkanContext.m_renderFinishedSemaphores[currentFrame]);

			m_vulkanContext.m_graphicsQueue.submit(submitInfo, m_vulkanContext.m_inFlightFences[currentFrame]);
		}

		void presentImage(uint32_t imageIndex)
		{
			vk::PresentInfoKHR presentInfo{
				m_vulkanContext.m_renderFinishedSemaphores[m_currentFrame],
				m_vulkanContext.m_swapchain,
				imageIndex};

			vk::Result result{};
			try
			{
				result = m_vulkanContext.m_presentQueue.presentKHR(presentInfo);
				if (result != vk::Result::eSuccess)
				{
					throw std::runtime_error("Failed to present swap chain image!");
				}
			}
			catch (std::exception const& exc)
			{
				std::cerr << exc.what();
				recreateSwapChain();
			}

			if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || framebufferResized)
			{
				framebufferResized = false;
				recreateSwapChain();
			}
			else if (result != vk::Result::eSuccess)
			{
				throw std::runtime_error("failed to present swap chain image!");
			}
		}

		void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex)
		{
			spdlog::info("Renderer::recordCommandBuffer()");

			commandBuffer.begin(vk::CommandBufferBeginInfo{});

			vk::Viewport viewport{0.0F,
								  0.0F,
								  static_cast<float>(m_vulkanContext.m_swapchainExtent.width),
								  static_cast<float>(m_vulkanContext.m_swapchainExtent.height),
								  0.0F,
								  1.0F};

			vk::Rect2D scissor{
				{0, 0},
				m_vulkanContext.m_swapchainExtent
			 };

			commandBuffer.setViewport(0, 1, &viewport);
			commandBuffer.setScissor(0, 1, &scissor);

			const vk::ClearColorValue clearColor(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
			const vk::ClearDepthStencilValue clearDepthStencil(1.0f, 0);

			std::array<vk::ClearValue, 2> clearValues = {clearColor, clearDepthStencil};

			vk::RenderPassBeginInfo renderPassInfo{
				m_vulkanContext.m_renderPass,
				m_vulkanContext.m_swapchainFramebuffers[imageIndex],
				vk::Rect2D{vk::Offset2D{0, 0}, m_vulkanContext.m_swapchainExtent},
				clearValues
			   };

			commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

			// Iterate over all materials used in the sceneNO o
			// for(const auto& material : m_materialManager->m_materials)
			//{
			//	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, material->m_pipeline);
			//
			//	//Iterate over all objects associated with the material
			//
			//}

			commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline.pipeline);

			// Object
			auto indicesSize = m_input.getValue().m_meshData.getIndicesPointList().size();
			vk::Buffer vertexBuffers[] = {m_pipeline.resources.vertexBuffer};
			vk::DeviceSize offsets[] = {0};
			commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
			commandBuffer.bindIndexBuffer(m_pipeline.resources.indexBuffer, 0, vk::IndexType::eUint32);
			commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipeline.pipelineLayout, 0, m_pipeline.resources.descriptorSets[m_currentFrame], {});
			commandBuffer.drawIndexed(static_cast<uint32_t>(indicesSize), 1, 0, 0, 0);


			commandBuffer.endRenderPass();
			commandBuffer.end();
		}

		// Implementation
		void createDebugMessageUtils()
		{
			pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(m_vulkanContext.m_instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
			if (!pfnVkCreateDebugUtilsMessengerEXT)
			{
				// TODO - Change it to something independent of iostream
				std::cout << "GetInstanceProcAddr: Unable to find "
							 "pfnVkCreateDebugUtilsMessengerEXT function."
						  << std::endl;
				exit(1);
			}

			pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(m_vulkanContext.m_instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
			if (!pfnVkDestroyDebugUtilsMessengerEXT)
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

			m_vulkanContext.m_debugMessenger = m_vulkanContext.m_instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
		}

		void createLogicalDevice()
		{
			std::vector<vk::QueueFamilyProperties> allQueueFamilies = m_vulkanContext.m_physicalDevice.getQueueFamilyProperties();

			float queuePriority = 1.0f;
			std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
			for(uint32_t i = 0; i < allQueueFamilies.size(); i++)
			{
				vk::DeviceQueueCreateInfo queueCreateInfoItem{
					{},
					i,
					1,
					&queuePriority};

				queueCreateInfo.push_back(queueCreateInfoItem);
			}

			std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"};

			std::array m_deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

			vk::PhysicalDeviceFeatures deviceFeatures{};
			vk::DeviceCreateInfo createInfo{
				{},
				queueCreateInfo,
				validationLayers,
				m_deviceExtensions,
				&deviceFeatures};

			m_vulkanContext.m_device = m_vulkanContext.m_physicalDevice.createDevice(createInfo);
		}

		void createSwapChain()
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_vulkanContext.m_physicalDevice, m_vulkanContext.m_surface);

			vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
			if (swapChainSupport.capabilities.maxImageCount > 0 &&
				imageCount > swapChainSupport.capabilities.maxImageCount)
			{
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			QueueFamily indices = QueueFamily::findQueueFamilies(m_vulkanContext.m_physicalDevice, m_vulkanContext.m_surface);
			std::vector<uint32_t> queueFamilyIndices = {indices.m_graphicsFamily.value(), indices.m_presentFamily.value()};

			vk::SharingMode sharingMode = (indices.m_graphicsFamily != indices.m_presentFamily) ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive;

			vk::SwapchainCreateInfoKHR createInfo{
				{},
				m_vulkanContext.m_surface,
				imageCount,
				surfaceFormat.format,
				surfaceFormat.colorSpace,
				extent,
				1,
				vk::ImageUsageFlagBits::eColorAttachment,
				sharingMode,
				queueFamilyIndices,
				swapChainSupport.capabilities.currentTransform,
				vk::CompositeAlphaFlagBitsKHR::eOpaque,
				presentMode,
				VK_TRUE,
				nullptr
			};

			m_vulkanContext.m_swapchain = m_vulkanContext.m_device.createSwapchainKHR(createInfo);
			m_vulkanContext.m_swapchainImages = m_vulkanContext.m_device.getSwapchainImagesKHR(m_vulkanContext.m_swapchain);
			m_vulkanContext.m_swapchainImageFormat = surfaceFormat.format;
			m_vulkanContext.m_swapchainExtent = extent;

			createImageViews();
		}

		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
		{
			for (const auto& availableFormat : availableFormats)
			{
				if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
					availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				{
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
		{
			for (const auto& availablePresentMode : availablePresentModes)
			{
				if (availablePresentMode == vk::PresentModeKHR::eMailbox)
				{
					return availablePresentMode;
				}
			}

			return vk::PresentModeKHR::eFifo;
		}

		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
		{

			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() ||
				capabilities.currentExtent.height != std::numeric_limits<uint32_t>::max() ||
				capabilities.currentExtent.width != 0 ||
				capabilities.currentExtent.height != 0)
			{
				return capabilities.currentExtent;
			}
			else
			{
				vk::Extent2D actualExtent = {m_width, m_height};

				actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return actualExtent;
			}
		}

		void createImageViews()
		{
			for (const auto& swapChainImage : m_vulkanContext.m_swapchainImages)
			{
				vk::ImageViewCreateInfo createInfo{
					vk::ImageViewCreateFlags{},
					swapChainImage,
					vk::ImageViewType::e2D,
					m_vulkanContext.m_swapchainImageFormat,
					vk::ComponentMapping{vk::ComponentSwizzle::eIdentity,
											 vk::ComponentSwizzle::eIdentity,
											 vk::ComponentSwizzle::eIdentity,
											 vk::ComponentSwizzle::eIdentity},
					vk::ImageSubresourceRange{vk::ImageAspectFlags{vk::ImageAspectFlagBits::eColor}, 0, 1, 0, 1}
				};

				m_vulkanContext.m_swapchainImageViews.emplace_back(m_vulkanContext.m_device.createImageView(createInfo));
			}
		}

		void createRenderPass()
		{
			vk::AttachmentDescription colorAttachment{{},
													  m_vulkanContext.m_swapchainImageFormat,
													  vk::SampleCountFlagBits::e1,
													  vk::AttachmentLoadOp::eClear,
													  vk::AttachmentStoreOp::eStore,
													  vk::AttachmentLoadOp::eDontCare,
													  vk::AttachmentStoreOp::eDontCare,
													  vk::ImageLayout::eUndefined,
													  vk::ImageLayout::ePresentSrcKHR};

			vk::AttachmentDescription depthAttachment{{},
													  findDepthFormat(),
													  vk::SampleCountFlagBits::e1,
													  vk::AttachmentLoadOp::eClear,
													  vk::AttachmentStoreOp::eDontCare,
													  vk::AttachmentLoadOp::eDontCare,
													  vk::AttachmentStoreOp::eDontCare,
													  vk::ImageLayout::eUndefined,
													  vk::ImageLayout::eDepthStencilAttachmentOptimal};

			vk::AttachmentReference colorAttachmentRef{0, vk::ImageLayout::eColorAttachmentOptimal};
			vk::AttachmentReference depthAttachmentRef{1, vk::ImageLayout::eDepthStencilAttachmentOptimal};

			vk::SubpassDescription subpass{vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorAttachmentRef, {}, &depthAttachmentRef, {}};

			vk::SubpassDependency dependency{VK_SUBPASS_EXTERNAL,
											 0,
											 vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
											 vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
											 vk::AccessFlagBits::eNoneKHR,
											 vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite};

			std::array<vk::AttachmentDescription, 2> attachments{colorAttachment, depthAttachment};

			// attachment [0] -> color
			// attachment  [1] -> depth
			vk::RenderPassCreateInfo renderPassInfo{vk::RenderPassCreateFlags(), attachments, subpass, dependency};

			m_vulkanContext.m_renderPass = m_vulkanContext.m_device.createRenderPass(renderPassInfo);
		}

		vk::Format findDepthFormat()
		{
			return findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
									   vk::ImageTiling::eOptimal,
									   vk::FormatFeatureFlagBits::eDepthStencilAttachment);
		}

		vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const
		{
			for (const auto& format : candidates)
			{
				vk::FormatProperties props{m_vulkanContext.m_physicalDevice.getFormatProperties(format)};

				if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
				{
					return format;
				}
				else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
				{
					return format;
				}
			}

			throw std::runtime_error("failed to find supported format!");
		}


		void createBuffer(vk::DeviceSize size,
						  vk::BufferUsageFlags usage,
						  vk::MemoryPropertyFlags properties,
						  vk::Buffer& buffer,
						  vk::DeviceMemory& bufferMemory)
		{
			vk::BufferCreateInfo bufferInfo{{}, size, usage, vk::SharingMode::eExclusive};

			buffer = m_vulkanContext.m_device.createBuffer(bufferInfo);

			vk::MemoryRequirements memoryRequirements = m_vulkanContext.m_device.getBufferMemoryRequirements(buffer);

			vk::MemoryAllocateInfo allocInfo{memoryRequirements.size, findMemoryType(memoryRequirements.memoryTypeBits, properties)};

			bufferMemory = m_vulkanContext.m_device.allocateMemory(allocInfo);

			m_vulkanContext.m_device.bindBufferMemory(buffer, bufferMemory, 0);
		}

		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
		{
			vk::PhysicalDeviceMemoryProperties memProperties = m_vulkanContext.m_physicalDevice.getMemoryProperties();

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			{
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}

		void createCommandPool()
		{
			QueueFamily queueFamily = QueueFamily::findQueueFamilies(m_vulkanContext.m_physicalDevice, m_vulkanContext.m_surface);

			vk::CommandPoolCreateInfo poolInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamily.m_graphicsFamily.value()};

			m_vulkanContext.m_commandPool = m_vulkanContext.m_device.createCommandPool(poolInfo);
		}

		void createFrameBuffer()
		{
			createDepthResources();

			m_vulkanContext.m_swapchainFramebuffers.reserve(m_vulkanContext.m_swapchainImageViews.size());

			for (const auto& swapChainImageView : m_vulkanContext.m_swapchainImageViews)
			{
				std::array<vk::ImageView, 2> attachments{swapChainImageView, m_vulkanContext.m_depthImageView};

				vk::FramebufferCreateInfo framebufferInfo{vk::FramebufferCreateFlags{},
														  m_vulkanContext.m_renderPass,
														  attachments,
														  m_vulkanContext.m_swapchainExtent.width,
														  m_vulkanContext.m_swapchainExtent.height,
														  1};

				m_vulkanContext.m_swapchainFramebuffers.emplace_back(m_vulkanContext.m_device.createFramebuffer(framebufferInfo));
			}
		}

		void createDepthResources()
		{
			vk::Format depthFormat = findDepthFormat();

			vk::Extent2D swapChainExtent = m_vulkanContext.m_swapchainExtent;

			createImage(swapChainExtent.width,
						swapChainExtent.height,
						depthFormat,
						vk::ImageTiling::eOptimal,
						vk::ImageUsageFlagBits::eDepthStencilAttachment,
						vk::MemoryPropertyFlagBits::eDeviceLocal,
						m_vulkanContext.m_depthImage,
						m_vulkanContext.m_depthImageMemory);

			m_vulkanContext.m_depthImageView = createImageView(m_vulkanContext.m_depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
		}

		void createImage(uint32_t width,
						 uint32_t height,
						 vk::Format format,
						 vk::ImageTiling tiling,
						 vk::ImageUsageFlags usage,
						 vk::MemoryPropertyFlags properties,
						 vk::Image& image,
						 vk::DeviceMemory& imageMemory)
		{

			vk::ImageCreateInfo imageInfo{
				{},
				vk::ImageType::e2D,
				format,
				{width, height, 1},
				1,
				1,
				vk::SampleCountFlagBits::e1,
				tiling,
				usage,
				vk::SharingMode::eExclusive,
				{},
				vk::ImageLayout::eUndefined
			   };

			image = m_vulkanContext.m_device.createImage(imageInfo);

			vk::MemoryRequirements m_memRequirements = m_vulkanContext.m_device.getImageMemoryRequirements(image);

			vk::MemoryAllocateInfo allocInfo{m_memRequirements.size, findMemoryType(m_memRequirements.memoryTypeBits, properties)};

			imageMemory = m_vulkanContext.m_device.allocateMemory(allocInfo);

			m_vulkanContext.m_device.bindImageMemory(image, imageMemory, 0);
		}

		vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const
		{
			vk::ImageViewCreateInfo viewInfo{
				{},
				image,
				vk::ImageViewType::e2D,
				format,
				{},
				{aspectFlags, 0, 1, 0, 1},
				{}
			  };

			return m_vulkanContext.m_device.createImageView(viewInfo);
		}

		void createCommandBuffers()
		{
			vk::CommandBufferAllocateInfo allocInfo{m_vulkanContext.m_commandPool,
													vk::CommandBufferLevel::ePrimary,
													static_cast<uint32_t>(m_vulkanContext.m_swapchainFramebuffers.size())};

			m_vulkanContext.m_commandBuffers = m_vulkanContext.m_device.allocateCommandBuffers(allocInfo);
		}

		void createSyncObjects()
		{
			m_vulkanContext.m_imageAvailableSemaphores.reserve(m_vulkanContext.MAX_FRAMES_IN_FLIGHT);
			m_vulkanContext.m_renderFinishedSemaphores.reserve(m_vulkanContext.MAX_FRAMES_IN_FLIGHT);
			m_vulkanContext.m_inFlightFences.reserve(m_vulkanContext.MAX_FRAMES_IN_FLIGHT);

			for (size_t i = 0; i < m_vulkanContext.MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_vulkanContext.m_imageAvailableSemaphores.emplace_back(m_vulkanContext.m_device.createSemaphore(vk::SemaphoreCreateInfo{}));
				m_vulkanContext.m_renderFinishedSemaphores.emplace_back(m_vulkanContext.m_device.createSemaphore(vk::SemaphoreCreateInfo{}));
				m_vulkanContext.m_inFlightFences.emplace_back(m_vulkanContext.m_device.createFence(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled}));
			}
		}

		void recreateSwapChain()
		{
			clearSwapChain();

			createSwapChain();
			createFrameBuffer();
		}

		void clearSwapChain()
		{
			// Framebuffers
			m_vulkanContext.m_device.waitIdle();
			m_vulkanContext.m_device.destroyImageView(m_vulkanContext.m_depthImageView);
			m_vulkanContext.m_device.destroyImage(m_vulkanContext.m_depthImage);
			m_vulkanContext.m_device.freeMemory(m_vulkanContext.m_depthImageMemory);

			for (auto& framebuffer : m_vulkanContext.m_swapchainFramebuffers)
			{
				m_vulkanContext.m_device.destroyFramebuffer(framebuffer);
			}
			m_vulkanContext.m_swapchainFramebuffers.clear();

			// Swapchain
			for (auto& imageView : m_vulkanContext.m_swapchainImageViews)
			{
				m_vulkanContext.m_device.destroyImageView(imageView);
			}
			m_vulkanContext.m_swapchainImageViews.clear();

			m_vulkanContext.m_device.destroySwapchainKHR(m_vulkanContext.m_swapchain);
		}

		// Temporary
		void updateVertexBuffer()
		{
			auto vertices = m_input.getValue().m_meshData.getVertexPointList();

			vk::BufferCreateInfo bufferInfo{{},
											sizeof(vertices[0]) * vertices.size(),
											vk::BufferUsageFlagBits::eVertexBuffer,
											vk::SharingMode::eExclusive};

			m_pipeline.resources.vertexBuffer = m_vulkanContext.m_device.createBuffer(bufferInfo);

			vk::MemoryRequirements memRequirements = m_vulkanContext.m_device.getBufferMemoryRequirements(m_pipeline.resources.vertexBuffer);

			vk::MemoryAllocateInfo allocInfo{memRequirements.size,
											 findMemoryType(memRequirements.memoryTypeBits,
															vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)};

			m_pipeline.resources.vertexBufferMemory = m_vulkanContext.m_device.allocateMemory(allocInfo);

			m_vulkanContext.m_device.bindBufferMemory(m_pipeline.resources.vertexBuffer, m_pipeline.resources.vertexBufferMemory, 0);

			void* data = m_vulkanContext.m_device.mapMemory(m_pipeline.resources.vertexBufferMemory, 0, bufferInfo.size);
			memcpy(data, vertices.data(), (size_t)bufferInfo.size);
			m_vulkanContext.m_device.unmapMemory(m_pipeline.resources.vertexBufferMemory);
		}

		void updateIndexBuffer()
		{
			auto m_indices = m_input.getValue().m_meshData.getIndicesPointList();
			vk::BufferCreateInfo bufferInfo{{},
											sizeof(m_indices[0]) * m_indices.size(),
											vk::BufferUsageFlagBits::eIndexBuffer,
											vk::SharingMode::eExclusive};

			m_pipeline.resources.indexBuffer = m_vulkanContext.m_device.createBuffer(bufferInfo);

			vk::MemoryRequirements memRequirements = m_vulkanContext.m_device.getBufferMemoryRequirements(m_pipeline.resources.indexBuffer);

			vk::MemoryAllocateInfo allocInfo{memRequirements.size,
											 findMemoryType(memRequirements.memoryTypeBits,
															vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)};

			m_pipeline.resources.indexBufferMemory = m_vulkanContext.m_device.allocateMemory(allocInfo);

			m_vulkanContext.m_device.bindBufferMemory(m_pipeline.resources.indexBuffer, m_pipeline.resources.indexBufferMemory, 0);

			void* data = m_vulkanContext.m_device.mapMemory(m_pipeline.resources.indexBufferMemory, 0, bufferInfo.size);
			memcpy(data, m_indices.data(), (size_t)bufferInfo.size);
			m_vulkanContext.m_device.unmapMemory(m_pipeline.resources.indexBufferMemory);
		}

		void updateScene(core::TypedInputHandler<Renderable> input)
		{
			m_input = input;

			// Initialize Shader
			spdlog::info("Renderer::updateScene() - Initialize Shader");
			spdlog::info("Vertex Shader: {}", m_input.getValue().m_vertexShader);
			spdlog::info("Fragment Shader: {}", m_input.getValue().m_fragmentShader);
			spdlog::info("Mesh: {}", m_input.getValue().m_meshData.getVertexPointList().size());
			spdlog::info("Mesh: {}", m_input.getValue().m_meshData.getIndicesPointList().size());

			m_pipeline = PipelineBuilder(m_vulkanContext)
							 .setVertexShader(m_input.getValue().m_vertexShader)
							 .setFragmentShader(m_input.getValue().m_fragmentShader)
							 .build();
		}

	  public:
	  	bool m_initialized = false;
		VulkanContext m_vulkanContext;
		Pipeline m_pipeline;
		PhysicalDevice m_physicalDevice;

		uint32_t m_currentFrame = 0;
		uint32_t framebufferResized = 0;

		uint32_t m_width = 800;
		uint32_t m_height = 600;

	
		std::unique_ptr<MaterialManager> m_materialManager;
		core::TypedInputHandler<Renderable> m_input;
	};

	/*---------------------*/
	/*-------Public--------*/
	/*---------------------*/
	Renderer::Renderer() :
		m_privateRenderer(std::make_unique<PrivateRenderer>())
	{
	}

	Renderer::~Renderer() = default;

	Renderer::Renderer(Renderer&&) noexcept = default;

	Renderer& Renderer::operator= (Renderer&&) noexcept = default;

	void Renderer::init()
	{
		m_privateRenderer->init();
	}

	void Renderer::render()
	{
		m_privateRenderer->render();
	}

	void Renderer::setSurface(vk::SurfaceKHR surface)
	{
		m_privateRenderer->initSurface(surface);
	}

	vk::Instance Renderer::getVulkanInstance() const
	{
		return m_privateRenderer->getVulkanInstance();
	}

	void Renderer::changeSwapchainExtent(uint32_t width, uint32_t height)
	{
		if (width > 0)
		{
			m_privateRenderer->m_width = width;
		}

		if (height > 0)
		{
			m_privateRenderer->m_height = height;
		}

		m_privateRenderer->recreateSwapChain();
	}

	const MaterialManager& Renderer::getMaterialManager() const
	{
		return *m_privateRenderer->m_materialManager.get();
	}

	void Renderer::updateScene()
	{
		// Renderable test = m_input.renderable;
		spdlog::info("Renderer::updateScene()");
		// Renderable test = m_input.renderable.getValue();
		// spdlog::info("Renderer: {}", test);
		// TODO improve this
		// m_privateRenderer->m_input = m_input.renderable;

		// spdlog::info("Renderer copied input");
		// spdlog::info("Renderer: {}", m_privateRenderer->m_input.getValue());

		// Initialize Shader

		if(m_privateRenderer->m_initialized)
		{
			m_privateRenderer->updateScene(m_input.renderable);
		}
		else
		{
			m_privateRenderer->m_input = m_input.renderable;
		}
		// Initialize Pipeline

		// Initialize Objects
	}

} // namespace st::renderer
