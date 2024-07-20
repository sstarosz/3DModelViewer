#ifndef ST_RENDERER_MATERIAL_HPP
#define ST_RENDERER_MATERIAL_HPP

#include "Core/Node.hpp"

#include <vulkan/vulkan.hpp>
#include <vector>

namespace st::renderer
{
    class Material
    {
    public:
        Material() = default;
        virtual ~Material() = default;

    public:
        vk::Pipeline m_pipeline;
        std::vector<std::shared_ptr<core::Node>> m_connectedNotes;
    };
} // namespace st::renderer


#endif // ST_RENDERER_MATERIAL_HPPs