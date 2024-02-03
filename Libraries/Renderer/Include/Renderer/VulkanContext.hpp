#ifndef ST_RENDERER_VULKANCONTEXT_HPP
#define ST_RENDERER_VULKANCONTEXT_HPP

#include <vulkan/vulkan.hpp>
#include <vector>

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

    //Swapchain
    vk::SwapchainKHR m_swapchain;
    std::vector<vk::Image> m_swapchainImages;
    vk::Format m_swapchainImageFormat;
    vk::Extent2D m_swapchainExtent;
    std::vector<vk::ImageView> m_swapchainImageViews;

    //Render pass
    vk::RenderPass m_renderPass;

    //Command pool
    vk::CommandPool m_commandPool;
    std::vector<vk::CommandBuffer> m_commandBuffers;

    //Framebuffers
    std::vector<vk::Framebuffer> m_swapchainFramebuffers;
    vk::Image m_depthImage;
    vk::DeviceMemory m_depthImageMemory;
    vk::ImageView m_depthImageView;

    //Synchronization
    std::vector<vk::Semaphore> m_imageAvailableSemaphores;
    std::vector<vk::Semaphore> m_renderFinishedSemaphores;
    std::vector<vk::Fence> m_inFlightFences;
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

#endif //ST_RENDERER_VULKANCONTEXT_HPP