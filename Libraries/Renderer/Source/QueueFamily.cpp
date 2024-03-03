#include "QueueFamily.hpp"

#include <vulkan/vulkan.hpp>

namespace st::renderer
{
	bool QueueFamily::isComplete() const
	{
		return m_graphicsFamily.has_value() && m_presentFamily.has_value();
	}

	QueueFamily QueueFamily::findQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
	{
		QueueFamily queueFamily;

		const auto queueFamilies = device.getQueueFamilyProperties();

		int i = 0;
		for (const auto& queueFamilyProperties : queueFamilies)
		{
			if (queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				queueFamily.m_graphicsFamily = i;
			}

			if (device.getSurfaceSupportKHR(i, surface))
			{
				queueFamily.m_presentFamily = i;
			}

			if (queueFamily.isComplete())
			{
				break;
			}

			i++;
		}

		return queueFamily;
	}

} // namespace st::renderer