#include "PhysicalDevice.hpp"
#include "QueueFamily.hpp"

#include <set>
#include <string>

namespace st::renderer
{

	PhysicalDevice::PhysicalDevice(VulkanContext& vulkanContext) :
		m_vulkanContext(vulkanContext)
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
			// Pick first that fullfil condtion
			if (isDeviceSuitable(device))
			{
				m_vulkanContext.m_physicalDevice = device;
				break;
			}
		}
	}


	

	//Check if device support all required extensions and has graphics queue family
	bool PhysicalDevice::isDeviceSuitable(const vk::PhysicalDevice& device) const
	{
		// TODO - Check if this function prefere real GPU than gpu inside cpu

		bool isGraphicsSupported = false;
		const auto queueFamily = device.getQueueFamilyProperties();
		for(const auto& queueFamilyProperties : queueFamily)
		{
			if (queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				isGraphicsSupported = true;
			}
		}

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		return isGraphicsSupported && extensionsSupported;
	}

	bool PhysicalDevice::checkDeviceExtensionSupport(const vk::PhysicalDevice& device) const
	{
		const std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

		std::set<std::string> requiredExtensions(m_deviceExtensions.begin(),
												 m_deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

} // namespace st::renderer