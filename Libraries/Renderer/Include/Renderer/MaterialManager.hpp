#ifndef ST_RENDERER_MATERIAL_MANAGER_HPP
#define ST_RENDERER_MATERIAL_MANAGER_HPP

#include "Material.hpp"
#include <vector>
#include <memory>

namespace st::renderer
{
    class MaterialManager
    {
    public:
        MaterialManager() = default;

        void createMaterial(Material& material) const
        {
            //m_materials.push_back(std::make_shared<Material>(material));
            //TODO implement
        }

    public:
        std::vector<std::shared_ptr<Material>> m_materials;
    };

} // namespace st::renderer

#endif // ST_RENDERER_MATERIAL_MANAGER_HPP