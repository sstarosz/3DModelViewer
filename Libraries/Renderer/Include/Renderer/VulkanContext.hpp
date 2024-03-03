#ifndef ST_RENDERER_VULKANCONTEXT_HPP
#define ST_RENDERER_VULKANCONTEXT_HPP

#include <vector>
#include <vulkan/vulkan.hpp>

namespace st::renderer
{

	struct VulkanContext
	{
		vk::Instance m_instance;
		vk::SurfaceKHR m_surface;
		vk::DebugUtilsMessengerEXT m_debugMessenger;

		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;

		// Swapchain
		vk::SwapchainKHR m_swapchain;
		std::vector<vk::Image> m_swapchainImages;
		vk::Format m_swapchainImageFormat;
		vk::Extent2D m_swapchainExtent;
		std::vector<vk::ImageView> m_swapchainImageViews;

		// Render pass
		vk::RenderPass m_renderPass;

		// Command pool
		vk::CommandPool m_commandPool;
		std::vector<vk::CommandBuffer> m_commandBuffers;

		// Framebuffers
		std::vector<vk::Framebuffer> m_swapchainFramebuffers;
		vk::Image m_depthImage;
		vk::DeviceMemory m_depthImageMemory;
		vk::ImageView m_depthImageView;

		// Synchronization
		std::vector<vk::Semaphore> m_imageAvailableSemaphores;
		std::vector<vk::Semaphore> m_renderFinishedSemaphores;
		std::vector<vk::Fence> m_inFlightFences;

		// Constants
		const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

		// TODO refactor
		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
		{
			vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			{
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}
	};

	struct GraphicsPipeline
	{
		vk::Pipeline m_graphicsPipeline;
		vk::PipelineLayout m_pipelineLayout;
		vk::PipelineCache m_pipelineCache;

		vk::DescriptorPool m_descriptorPool;
		vk::DescriptorSetLayout m_descriptorSetLayout;

		std::vector<vk::Buffer> m_uniformBuffers;
		std::vector<vk::DeviceMemory> m_uniformBuffersMemory;

		vk::Sampler m_textureSampler;
	};

} // namespace st::renderer

#endif // ST_RENDERER_VULKANCONTEXT_HPP