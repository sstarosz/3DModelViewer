#include "Renderer.hpp"
#include "VulkanContext.hpp"
#include "PhysicalDevice.hpp"
#include "QueueFamily.hpp"
#include <vector>

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>
#include <sstream>
#include <array>
#include <numeric>
#include <filesystem>
#include <fstream>
#include <Eigen/Core>


PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

//TODO - change it to class method
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

    struct Vertex
    {
        Eigen::Vector3f m_position;
        Eigen::Vector2f m_texCoord;
        Eigen::Vector3f m_color;
        Eigen::Vector3f m_normal;


        static vk::VertexInputBindingDescription getBindingDescription()
        {
             vk::VertexInputBindingDescription bindingDescription{0,
                                                                  sizeof(Vertex),
                                                                  vk::VertexInputRate::eVertex
                                                                  };

            return bindingDescription;
        }

        static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions()
        {
            std::array<vk::VertexInputAttributeDescription, 4> attributeDescriptions{
            vk::VertexInputAttributeDescription{
                0,
                0,
                vk::Format::eR32G32B32Sfloat,
                static_cast<uint32_t>(offsetof(Vertex, m_position))
                },
                vk::VertexInputAttributeDescription{
                    1,
                    0,
                    vk::Format::eR32G32Sfloat,
                    static_cast<uint32_t>(offsetof(Vertex, m_texCoord))
                },
                vk::VertexInputAttributeDescription{
                    2,
                    0,
                    vk::Format::eR32G32B32Sfloat,
                    static_cast<uint32_t>(offsetof(Vertex, m_color))
                },
                vk::VertexInputAttributeDescription {
                    3,
                    0,
                    vk::Format::eR32G32B32Sfloat,
                    static_cast<uint32_t>(offsetof(Vertex, m_normal))
                }
            };

            return attributeDescriptions;
        }

        bool operator==(const Vertex&) const = default;
        auto operator<=>(const Vertex&) const = default;
    };

    struct UniformBufferObject
    {
        Eigen::Matrix4f m_model;
        Eigen::Matrix4f m_view;
        Eigen::Matrix4f m_proj;
    };



	VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
						   VkDebugUtilsMessageTypeFlagsEXT messageType,
						   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
						   void* pUserData)
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

    }

    ~PrivateRenderer()
    {

    }

    vk::Instance createInstance()
    {
        vk::ApplicationInfo appInfo {
            "St Model Viewer",
            VK_MAKE_VERSION(1, 0, 0),
            "St Model Viewer",
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_3
        };


        std::vector<const char*> extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,

            VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        };

        std::vector<const char*> layers = {
            "VK_LAYER_KHRONOS_validation",
        };


        vk::InstanceCreateInfo createInfo{
            {},
            &appInfo,
            layers,
            extensions
        };

        m_vulkanContext.m_instance = vk::createInstance(createInfo);
        return m_vulkanContext.m_instance;
    }

    void init()
    {
        createDebugMessageUtils();
        m_physicalDevice.initialize();
        createLogicalDevice();
        createSwapChain();
        createRenderPass();
        createGraphicsPipeline();
        createCommandPool();
        createFrameBuffer();
        createCommandBuffers();
        createSyncObjects();

    }

    void waitForPreviousFrame()
    {
        auto resultFence = m_vulkanContext.m_device.waitForFences(m_vulkanContext.m_inFlightFences.at(currentFrame), 
                                                                  VK_TRUE, 
                                                                  UINT64_MAX);
        if (resultFence != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed to wait for fences!");
        }

        //TODO - Check for other results:
        //vk::Result::eTimeout
        //vk::Result::eErrorDeviceLost
        //vk::Result::eErrorOutOfDeviceMemory
    }

    uint32_t acquireSwapChainImage()
    {
        auto [result, imageIndex] = m_vulkanContext.m_device.acquireNextImageKHR(m_vulkanContext.m_swapchain, 
                                                                                 UINT64_MAX, 
                                                                                 m_vulkanContext.m_imageAvailableSemaphores[currentFrame], 
                                                                                 VK_NULL_HANDLE);
        if (result == vk::Result::eErrorOutOfDateKHR) 
        {
            recreateSwapChain();
            return acquireSwapChainImage();
        } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) 
        {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        //TODO - Check for other results:
        //vk::Result::eTimeout
        //vk::Result::eNotReady
        //vk::Result::eSuboptimalKHR

        //vk::Result::eErrorOutOfHostMemory
        //vk::Result::eErrorOutOfDeviceMemory
        //vk::Result::eErrorDeviceLost
        //vk::Result::eErrorOutOfDateKHR
        //vk::Result::eErrorSurfaceLostKHR
        //vk::Result::eErrorFullScreenExclusiveModeLostEXT

        return imageIndex;
    }

    void resetCommandBuffer(uint32_t currentFrame)
    {
        m_vulkanContext.m_commandBuffers[currentFrame].reset(vk::CommandBufferResetFlagBits::eReleaseResources);
    }

    void render()
    {
		//Wait for previous frame to finish
        waitForPreviousFrame();

        //Reset the fence for the current frame
        m_vulkanContext.m_device.resetFences(m_vulkanContext.m_inFlightFences.at(currentFrame));

        //Acquire swap chain image
        uint32_t imageIndex = acquireSwapChainImage();


        //Reset the command buffer for the current frame
        resetCommandBuffer(currentFrame);

        // Record command for the current frame
        recordCommandBuffer(m_vulkanContext.m_commandBuffers[currentFrame], imageIndex);

        

        //Submit command buffer
        submitCommandBuffer(currentFrame);

        //Present image
        presentImage(imageIndex);

        //Advance to next frame
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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
        vk::PresentInfoKHR presentInfo {
            m_vulkanContext.m_renderFinishedSemaphores[currentFrame],
            m_vulkanContext.m_swapchain,
            imageIndex
        };


        vk::Result result;
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
        commandBuffer.begin(vk::CommandBufferBeginInfo {});

        const vk::ClearColorValue clearColor(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        const vk::ClearDepthStencilValue clearDepthStencil(1.0f, 0);

        std::array<vk::ClearValue, 2> clearValues = { clearColor, clearDepthStencil };

		vk::RenderPassBeginInfo renderPassInfo { m_vulkanContext.m_renderPass,
												 m_vulkanContext.m_swapchainFramebuffers[imageIndex],
												 vk::Rect2D((0, 0), m_vulkanContext.m_swapchainExtent),
												 clearValues };

		commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

        vk::Viewport viewport { 0.0F, 
                                0.0F, 
                                static_cast<float>(m_vulkanContext.m_swapchainExtent.width), 
                                static_cast<float>(m_vulkanContext.m_swapchainExtent.height), 
                                0.0F, 
                                1.0F };

		vk::Rect2D scissor {
			{0, 0},
			m_vulkanContext.m_swapchainExtent
		};
		commandBuffer.setViewport(0, 1, &viewport);
		commandBuffer.setScissor(0, 1, &scissor);



        commandBuffer.endRenderPass();
		commandBuffer.end();
    }




//Implementation
    void createDebugMessageUtils()
    {
        pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(m_vulkanContext.m_instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
		if (!pfnVkCreateDebugUtilsMessengerEXT)
		{
			//TODO - Change it to something independent of iostream
			std::cout << "GetInstanceProcAddr: Unable to find "
						 "pfnVkCreateDebugUtilsMessengerEXT function."
					  << std::endl;
			exit(1);
		}

		pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(m_vulkanContext.m_instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
		if (!pfnVkDestroyDebugUtilsMessengerEXT)
		{
			//TODO - Change it to something independent of iostream
			std::cout << "GetInstanceProcAddr: Unable to find "
						 "pfnVkDestroyDebugUtilsMessengerEXT function."
					  << std::endl;
			exit(1);
		}


        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags{
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
        };

        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags{
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
        };


        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT { 
            {}, 
            severityFlags, 
            messageTypeFlags, 
            &debugCallback };


        m_vulkanContext.m_debugMessenger = m_vulkanContext.m_instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);  
    }

    void createLogicalDevice()
    {
        QueueFamily indices = QueueFamily::findQueueFamilies(m_vulkanContext.m_physicalDevice, m_vulkanContext.m_surface);

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
        std::set<uint32_t> uniqueQueueFamilies = { indices.m_graphicsFamily.value(), indices.m_presentFamily.value() };

        float queuePriority = 1.0f;
        for(const auto& queueFamily : uniqueQueueFamilies)
        {
            vk::DeviceQueueCreateInfo queueCreateInfoItem{
                {},
                queueFamily,
                1,
                &queuePriority
            };

            queueCreateInfo.push_back(queueCreateInfoItem);
        }

        std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        std::array m_deviceExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        vk::PhysicalDeviceFeatures deviceFeatures{};
        vk::DeviceCreateInfo createInfo{
            {},
            queueCreateInfo,
            validationLayers,
            m_deviceExtensions,
            &deviceFeatures
        };

        m_vulkanContext.m_device = m_vulkanContext.m_physicalDevice.createDevice(createInfo);
        m_vulkanContext.m_graphicsQueue = m_vulkanContext.m_device.getQueue(indices.m_graphicsFamily.value(), 0);
        m_vulkanContext.m_presentQueue = m_vulkanContext.m_device.getQueue(indices.m_presentFamily.value(), 0); 
    }

    void createSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_vulkanContext.m_physicalDevice, m_vulkanContext.m_surface);

        vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if(swapChainSupport.capabilities.maxImageCount > 0 && 
           imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        QueueFamily indices = QueueFamily::findQueueFamilies(m_vulkanContext.m_physicalDevice, m_vulkanContext.m_surface);
        std::vector<uint32_t> queueFamilyIndices = { indices.m_graphicsFamily.value(), indices.m_presentFamily.value() };

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
        for(const auto& availableFormat : availableFormats)
        {
            if(availableFormat.format == vk::Format::eB8G8R8A8Srgb && 
               availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
    {
        for(const auto& availablePresentMode : availablePresentModes)
        {
            if(availablePresentMode == vk::PresentModeKHR::eMailbox)
            {
                return availablePresentMode;
            }
        }

        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
    {
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            vk::Extent2D actualExtent = { m_width, m_height };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void createImageViews()
    {
		for (const auto& swapChainImage :  m_vulkanContext.m_swapchainImages)
		{
			vk::ImageViewCreateInfo createInfo {
				vk::ImageViewCreateFlags {},
				swapChainImage,
				vk::ImageViewType::e2D,
				 m_vulkanContext.m_swapchainImageFormat,
				vk::ComponentMapping { vk::ComponentSwizzle::eIdentity,
										  vk::ComponentSwizzle::eIdentity,
										  vk::ComponentSwizzle::eIdentity,
										  vk::ComponentSwizzle::eIdentity },
				vk::ImageSubresourceRange { vk::ImageAspectFlags { vk::ImageAspectFlagBits::eColor }, 0, 1, 0, 1 }
			};

			 m_vulkanContext.m_swapchainImageViews.emplace_back( m_vulkanContext.m_device.createImageView(createInfo));
		}
    }

    void createRenderPass()
    {
        vk::AttachmentDescription colorAttachment { {}, 
                                                    m_vulkanContext.m_swapchainImageFormat,
													vk::SampleCountFlagBits::e1,      
                                                    vk::AttachmentLoadOp::eClear,
													vk::AttachmentStoreOp::eStore,    
                                                    vk::AttachmentLoadOp::eDontCare,
													vk::AttachmentStoreOp::eDontCare, 
                                                    vk::ImageLayout::eUndefined,
													vk::ImageLayout::ePresentSrcKHR 
                                                    };

		vk::AttachmentDescription depthAttachment { {},
                                                    findDepthFormat(),
                                                    vk::SampleCountFlagBits::e1,      
                                                    vk::AttachmentLoadOp::eClear,
                                                    vk::AttachmentStoreOp::eDontCare,    
                                                    vk::AttachmentLoadOp::eDontCare,
                                                    vk::AttachmentStoreOp::eDontCare, 
                                                    vk::ImageLayout::eUndefined,
                                                    vk::ImageLayout::eDepthStencilAttachmentOptimal 
                                                    };


		vk::AttachmentReference colorAttachmentRef { 0, vk::ImageLayout::eColorAttachmentOptimal };
		vk::AttachmentReference depthAttachmentRef { 1, vk::ImageLayout::eDepthStencilAttachmentOptimal };


		vk::SubpassDescription subpass { vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, {}, colorAttachmentRef, {}, &depthAttachmentRef, {} };


		vk::SubpassDependency dependency { VK_SUBPASS_EXTERNAL,
										   0,
										   vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
										   vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
										   vk::AccessFlagBits::eNoneKHR,
										   vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite };


		std::array<vk::AttachmentDescription, 2> attachments { colorAttachment, depthAttachment };


		//attachment [0] -> color
		//atachment  [1] -> depth
		vk::RenderPassCreateInfo renderPassInfo { vk::RenderPassCreateFlags(), attachments, subpass, dependency };


		m_vulkanContext.m_renderPass = m_vulkanContext.m_device.createRenderPass(renderPassInfo);
    }

    vk::Format findDepthFormat()
    {
        return findSupportedFormat({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
								   vk::ImageTiling::eOptimal,
								   vk::FormatFeatureFlagBits::eDepthStencilAttachment);
    }

	vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const
	{
		for (const auto& format : candidates)
		{
			vk::FormatProperties props { m_vulkanContext.m_physicalDevice.getFormatProperties(format) };

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

    void createGraphicsPipeline()
    {
        createTextureSampler();
		createUniformBuffers();
		createDescriptorPool();
		createDescriptorSetLayout(); // must stay in pipline creation

		auto vertShaderCode = readFile("../Assets/Shaders/vert.spv");
		auto fragShaderCode = readFile("../Assets/Shaders/frag.spv");


		vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);


		vk::PipelineShaderStageCreateInfo vertShaderStageInfo { {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main" };

		vk::PipelineShaderStageCreateInfo fragShaderStageInfo { {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main" };

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages { vertShaderStageInfo, fragShaderStageInfo };


		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo { {}, bindingDescription, attributeDescriptions };

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly { vk::PipelineInputAssemblyStateCreateFlags {}, vk::PrimitiveTopology::eTriangleList, VK_FALSE };


		vk::PipelineViewportStateCreateInfo viewportState { vk::PipelineViewportStateCreateFlags {}, 1, {}, 1, {} };

		vk::PipelineRasterizationStateCreateInfo rasterizer { vk::PipelineRasterizationStateCreateFlags {},
															  VK_FALSE,
															  VK_FALSE,
															  vk::PolygonMode::eFill,
															  vk::CullModeFlagBits::eBack,
															  vk::FrontFace::eCounterClockwise,
															  VK_FALSE,
															  0.0f,
															  0.0f,
															  0.0f,
															  1.0F };

		vk::PipelineMultisampleStateCreateInfo multisampling { vk::PipelineMultisampleStateCreateFlags {}, vk::SampleCountFlagBits::e1, VK_FALSE };

		vk::PipelineDepthStencilStateCreateInfo depthStencil { {}, true, true, vk::CompareOp::eLess, false, false };

	    std::vector<vk::DynamicState> m_dynamicStateEnables { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

		vk::PipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo { {}, m_dynamicStateEnables };

		vk::PipelineColorBlendAttachmentState colorBlendAttachment { VK_FALSE,
																	 vk::BlendFactor::eZero,
																	 vk::BlendFactor::eZero,
																	 vk::BlendOp::eAdd,
																	 vk::BlendFactor::eZero,
																	 vk::BlendFactor::eZero,
																	 vk::BlendOp::eAdd,
																	 vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
																		 vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };


		vk::PipelineColorBlendStateCreateInfo colorBlending {
			vk::PipelineColorBlendStateCreateFlags {},
			VK_FALSE,
			vk::LogicOp::eCopy,
			colorBlendAttachment,
			{ 0.0f, 0.0f, 0.0f, 0.0f }
		};


		vk::PipelineLayoutCreateInfo pipelineLayoutInfo { vk::PipelineLayoutCreateFlags {}, m_graphicsPipeline.m_descriptorSetLayout };

		m_graphicsPipeline.m_pipelineLayout = m_vulkanContext.m_device.createPipelineLayout(pipelineLayoutInfo);

		vk::GraphicsPipelineCreateInfo pipelineInfo { vk::PipelineCreateFlags {},
													  shaderStages,
													  &vertexInputInfo,
													  &inputAssembly,
													  {},
													  &viewportState,
													  &rasterizer,
													  &multisampling,
													  &depthStencil,
													  &colorBlending,
													  &m_pipelineDynamicStateCreateInfo,
													  m_graphicsPipeline.m_pipelineLayout,
													  m_vulkanContext.m_renderPass };


		m_graphicsPipeline.m_pipelineCache = m_vulkanContext.m_device.createPipelineCache(vk::PipelineCacheCreateInfo());
		m_graphicsPipeline.m_graphicsPipeline = m_vulkanContext.m_device.createGraphicsPipeline(m_graphicsPipeline.m_pipelineCache, pipelineInfo).value;


		//Note VkShaderModule is passed into pipline and are not longer available trought object they are used to create
		//If ther are used later, then they must not be destroyed
		m_vulkanContext.m_device.destroy(fragShaderModule);
		m_vulkanContext.m_device.destroy(vertShaderModule);
    }

    void createTextureSampler()
    {
        vk::PhysicalDeviceProperties properties = m_vulkanContext.m_physicalDevice.getProperties();

		vk::SamplerCreateInfo sampleInfo {
			{},
			vk::Filter::eLinear,
			vk::Filter::eLinear,
			vk::SamplerMipmapMode::eLinear,
			vk::SamplerAddressMode::eRepeat,
			vk::SamplerAddressMode::eRepeat,
			vk::SamplerAddressMode::eRepeat,
			0.0f,
			false,
			properties.limits.maxSamplerAnisotropy,
			false,
			vk::CompareOp::eAlways,
			0.0f,
			0.0f,
			vk::BorderColor::eIntOpaqueBlack,
			false,
		};

		m_graphicsPipeline.m_textureSampler = m_vulkanContext.m_device.createSampler(sampleInfo);
    }

    void createUniformBuffers()
    {
        const VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		m_graphicsPipeline.m_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		m_graphicsPipeline.m_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			createBuffer(bufferSize,
                         vk::BufferUsageFlagBits::eUniformBuffer,
                         vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                         m_graphicsPipeline.m_uniformBuffers[i],
                         m_graphicsPipeline.m_uniformBuffersMemory[i]);
		}
    }

    void createDescriptorPool()
    {
		std::array<vk::DescriptorPoolSize, 2> poolsSize {
			vk::DescriptorPoolSize { vk::DescriptorType::eUniformBuffer,        MAX_FRAMES_IN_FLIGHT * 2},
			vk::DescriptorPoolSize { vk::DescriptorType::eCombinedImageSampler, MAX_FRAMES_IN_FLIGHT * 2}
		};

		const vk::DescriptorPoolCreateInfo poolInfo { {}, MAX_FRAMES_IN_FLIGHT * 2, poolsSize };
		m_graphicsPipeline.m_descriptorPool = m_vulkanContext.m_device.createDescriptorPool(poolInfo);
    }

    void createDescriptorSetLayout()
    {
		vk::DescriptorSetLayoutBinding uboLayoutBinding { 0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex };

		vk::DescriptorSetLayoutBinding samplerLayoutBinding { 1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment };

		std::array<vk::DescriptorSetLayoutBinding, 2> bindings { uboLayoutBinding, samplerLayoutBinding };
		vk::DescriptorSetLayoutCreateInfo layoutInfo { {}, bindings };

		m_graphicsPipeline.m_descriptorSetLayout = m_vulkanContext.m_device.createDescriptorSetLayout(layoutInfo);
    }

    std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    vk::ShaderModule createShaderModule(const std::vector<char>& code)
    {
        vk::ShaderModuleCreateInfo createInfo { vk::ShaderModuleCreateFlags {}, code.size(), reinterpret_cast<const uint32_t*>(code.data()) };

        vk::ShaderModule shaderModule = m_vulkanContext.m_device.createShaderModule(createInfo);

        return shaderModule;
    }

    vk::VertexInputBindingDescription getBindingDescription()
    {
        vk::VertexInputBindingDescription bindingDescription{
            0,
            sizeof(Vertex),
            vk::VertexInputRate::eVertex
        };

        return bindingDescription;
    }

    void createBuffer(vk::DeviceSize size,
                      vk::BufferUsageFlags usage,
                      vk::MemoryPropertyFlags properties,
                      vk::Buffer& buffer,
                      vk::DeviceMemory& bufferMemory)
    {
		vk::BufferCreateInfo bufferInfo { {}, size, usage, vk::SharingMode::eExclusive };

		buffer = m_vulkanContext.m_device.createBuffer(bufferInfo);

		vk::MemoryRequirements memoryRequirements = m_vulkanContext.m_device.getBufferMemoryRequirements(buffer);

		vk::MemoryAllocateInfo allocInfo { memoryRequirements.size, findMemoryType(memoryRequirements.memoryTypeBits, properties) };

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

        vk::CommandPoolCreateInfo poolInfo {  vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamily.m_graphicsFamily.value() };

        m_vulkanContext.m_commandPool = m_vulkanContext.m_device.createCommandPool(poolInfo);
    }

    void createFrameBuffer()
    {
        createDepthResources();

		m_vulkanContext.m_swapchainFramebuffers.reserve(m_vulkanContext.m_swapchainImageViews.size());

		for (const auto& swapChainImageView : m_vulkanContext.m_swapchainImageViews)
		{
			std::array<vk::ImageView, 2> attachments { swapChainImageView, m_vulkanContext.m_depthImageView };

			vk::FramebufferCreateInfo framebufferInfo { vk::FramebufferCreateFlags {},
														m_vulkanContext.m_renderPass,
														attachments,
														m_vulkanContext.m_swapchainExtent.width,
														m_vulkanContext.m_swapchainExtent.height,
														1 };

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

		vk::ImageCreateInfo imageInfo {
			{},
            vk::ImageType::e2D,          format, { width, height, 1 },
            1, 1, vk::SampleCountFlagBits::e1, tiling,
			usage, vk::SharingMode::eExclusive, {},
            vk::ImageLayout::eUndefined
		};

		image = m_vulkanContext.m_device.createImage(imageInfo);

		vk::MemoryRequirements m_memRequirements = m_vulkanContext.m_device.getImageMemoryRequirements(image);

		vk::MemoryAllocateInfo allocInfo { m_memRequirements.size, findMemoryType(m_memRequirements.memoryTypeBits, properties) };

		imageMemory = m_vulkanContext.m_device.allocateMemory(allocInfo);

		m_vulkanContext.m_device.bindImageMemory(image, imageMemory, 0);
	}

    vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const
	{
		vk::ImageViewCreateInfo viewInfo 
        {
			{},
            image, vk::ImageViewType::e2D, format, {},
            { aspectFlags, 0, 1, 0, 1 },
            {}
		};

		return m_vulkanContext.m_device.createImageView(viewInfo);
	}

    void createCommandBuffers()
    {
        vk::CommandBufferAllocateInfo allocInfo { m_vulkanContext.m_commandPool,
												  vk::CommandBufferLevel::ePrimary,
												  static_cast<uint32_t>(m_vulkanContext.m_swapchainFramebuffers.size()) };

		m_vulkanContext.m_commandBuffers = m_vulkanContext.m_device.allocateCommandBuffers(allocInfo);
    }

    void createSyncObjects()
    {
        m_vulkanContext.m_imageAvailableSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
		m_vulkanContext.m_renderFinishedSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
		m_vulkanContext.m_inFlightFences.reserve(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_vulkanContext.m_imageAvailableSemaphores.emplace_back(m_vulkanContext.m_device.createSemaphore(vk::SemaphoreCreateInfo {}));
			m_vulkanContext.m_renderFinishedSemaphores.emplace_back(m_vulkanContext.m_device.createSemaphore(vk::SemaphoreCreateInfo {}));
			m_vulkanContext.m_inFlightFences.emplace_back(m_vulkanContext.m_device.createFence(vk::FenceCreateInfo { vk::FenceCreateFlagBits::eSignaled }));
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
        //Framebuffers
        m_vulkanContext.m_device.waitIdle();
        m_vulkanContext.m_device.destroyImageView(m_vulkanContext.m_depthImageView);
        m_vulkanContext.m_device.destroyImage(m_vulkanContext.m_depthImage);
        m_vulkanContext.m_device.freeMemory(m_vulkanContext.m_depthImageMemory);

        for (auto& framebuffer : m_vulkanContext.m_swapchainFramebuffers)
        {
            m_vulkanContext.m_device.destroyFramebuffer(framebuffer);
        }
        m_vulkanContext.m_swapchainFramebuffers.clear();

        //Swapchain
        for(auto& imageView : m_vulkanContext.m_swapchainImageViews)
        {
            m_vulkanContext.m_device.destroyImageView(imageView);
        }
        m_vulkanContext.m_swapchainImageViews.clear();

        m_vulkanContext.m_device.destroySwapchainKHR(m_vulkanContext.m_swapchain);
    }

public:
    VulkanContext m_vulkanContext;
    GraphicsPipeline m_graphicsPipeline;
    PhysicalDevice m_physicalDevice;

    const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;
    uint32_t framebufferResized = 0;

    uint32_t m_width = 800;
    uint32_t m_height = 600;
};









/*---------------------*/
/*-------Public--------*/
/*---------------------*/
Renderer::Renderer()
    : m_privateRenderer(std::make_unique<PrivateRenderer>())
{
}

Renderer::~Renderer() = default;

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
    m_privateRenderer->m_vulkanContext.m_surface = surface;
}

vk::Instance Renderer::createInstance() const
{
    return m_privateRenderer->createInstance();
}

void Renderer::changeSwapchainExtent(uint32_t width, uint32_t height)
{
    m_privateRenderer->m_width = width;
    m_privateRenderer->m_height = height;

    m_privateRenderer->recreateSwapChain();
}


} // namespace st::renderer
