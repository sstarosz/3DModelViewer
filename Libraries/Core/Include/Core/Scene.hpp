#ifndef ST_CORE_SCENE_HPP
#define ST_CORE_SCENE_HPP

#include <memory>
#include "Camera.hpp"

namespace st::core
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;

        void addModel();
        void addCamera(const Camera& camera);

    private:
        class PrivateScene;
        std::unique_ptr<PrivateScene> m_privateScene;
    };
} // namespace st::core

#endif // ST_CORE_SCENE_HPP
