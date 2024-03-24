#ifndef ST_CORE_SCENE_HPP
#define ST_CORE_SCENE_HPP

#include <memory>

namespace st::core
{
    class Camera;
} // namespace st::core

namespace st::core
{


    class Scene
    {
    public:
        Scene();
        ~Scene();
        Scene(const Scene&) = delete;
        Scene(Scene&&) noexcept;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) noexcept;

        void addModel();
        void addCamera(Camera&& camera);

    private:
        class PrivateScene;
        std::unique_ptr<PrivateScene> m_privateScene;
    };
} // namespace st::core

#endif // ST_CORE_SCENE_HPP
