#include "PhysicalDevice.hpp"
#include "QueueFamily.hpp"

#include <set>
#include <string>

namespace st::renderer
{

PhysicalDevice::PhysicalDevice(VulkanContext& vulkanContext)
    : m_vulkanContext(vulkanContext)
{
}

const vk::PhysicalDevice PhysicalDevice::getPhysicalDevice() const
{
    return m_vulkanContext.m_physicalDevice;
}


void PhysicalDevice::initialize()
{
    std::vector<vk::PhysicalDevice> devices = m_vulkanContext.m_instance.enumeratePhysicalDevices();

    if (devices.empty())
    {
        throw std::runtime_error("Failed to find GPU's with Vulkan support!");
    }

    for (const auto& device : devices)
    {
        // Check if devices contain all required functionality
        //Pick first that fullfil condtion
        if (isDeviceSuitable(device))
        {
            m_vulkanContext.m_physicalDevice = device;
            break;
        }
    }
}

bool PhysicalDevice::isDeviceSuitable(const vk::PhysicalDevice& device) const
{
    //TODO - Check if this function prefere real GPU than gpu inside cpu

    QueueFamily indices = QueueFamily::findQueueFamilies(device, m_vulkanContext.m_surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        auto availableFormats = device.getSurfaceFormatsKHR(m_vulkanContext.m_surface);
        auto availablePresentModes = device.getSurfacePresentModesKHR(m_vulkanContext.m_surface);
        swapChainAdequate = !availableFormats.empty() && !availablePresentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool PhysicalDevice::checkDeviceExtensionSupport(const vk::PhysicalDevice& device) const
{
    const std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


} // namespace st::renderer