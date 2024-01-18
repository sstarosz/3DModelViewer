#ifndef ST_RENDERER_VULKANCONTEXT_HPP
#define ST_RENDERER_VULKANCONTEXT_HPP

#include <vulkan/vulkan.hpp>

namespace st::renderer
{

struct VulkanContext
{
    vk::Instance m_instance;
    vk::SurfaceKHR m_surface;
    vk::DebugUtilsMessengerEXT m_debugMessenger;

    vk::PhysicalDevice m_physicalDevice;
};



} // namespace st::renderer

#endif //ST_RENDERER_VULKANCONTEXT_HPP