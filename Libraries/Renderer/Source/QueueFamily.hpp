#ifndef ST_RENDERER_QUEUEFAMILY_HPP
#define ST_RENDERER_QUEUEFAMILY_HPP

#include <optional>

namespace vk
{
	class PhysicalDevice;
	class SurfaceKHR;
}

namespace st::renderer
{

struct QueueFamily
{
    std::optional<uint32_t> m_graphicsFamily = -1;
    std::optional<uint32_t> m_presentFamily = -1;

    [[nodiscard]] bool isComplete() const;

    static QueueFamily findQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
};

} // namespace st::renderer

#endif //ST_RENDERER_QUEUEFAMILY_HPP