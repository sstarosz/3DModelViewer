#ifndef ST_RENDERER_PHYSICALDEVICE_HPP
#define ST_RENDERER_PHYSICALDEVICE_HPP

#include "VulkanContext.hpp"
#include <vulkan/vulkan.hpp>
#include <array>

namespace st::renderer
{

class PhysicalDevice
{
public:
    PhysicalDevice(VulkanContext& vulkanContext);
    
    void initialize();

    [[nodiscard]]
    const vk::PhysicalDevice getPhysicalDevice() const;

private:
    VulkanContext& m_vulkanContext;
    constexpr static std::array m_deviceExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };


    bool isDeviceSuitable(const vk::PhysicalDevice& device) const;
    bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device) const;
    bool checkQueueFamiliesSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface) const;
};

} // namespace st::renderer

#endif //ST_RENDERER_PHYSICALDEVICE_HPP
