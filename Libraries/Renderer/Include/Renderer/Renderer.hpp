#ifndef ST_RENDERER_RENDERER_HPP
#define ST_RENDERER_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include <memory>

namespace st::renderer
{

class Renderer
{
public:
    Renderer();
    ~Renderer();


    
    void init();
    void render();


    vk::Instance createInstance() const;
    void setSurface(vk::SurfaceKHR surface);

    void changeSwapchainExtent(uint32_t width, uint32_t height);

private:
    class PrivateRenderer;
    std::unique_ptr<PrivateRenderer> m_privateRenderer;
};





} // namespace st::renderer

#endif //ST_RENDERER_RENDERER_HPP