#include "Renderer.hpp"
#include "VulkanContext.hpp"
#include "PhysicalDevice.hpp"

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>


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
        pickPhysicalDevice();
    }

    void render()
    {

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

    void pickPhysicalDevice()
    {
        std::vector<vk::PhysicalDevice> physicalDevices = m_vulkanContext.m_instance.enumeratePhysicalDevices();
        if(physicalDevices.empty())
        {
            throw std::runtime_error("Failed to find GPU's with Vulkan support!");
        }

        for(const auto& physicalDevice : physicalDevices)
        {
            vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
            std::cout << "Device name: " << properties.deviceName << std::endl;
        }
    }

public:
    VulkanContext m_vulkanContext;
    PhysicalDevice m_physicalDevice;

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



} // namespace st::renderer
