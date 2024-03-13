#include "Scene.hpp"
#include "Camera.hpp"

#include <vector>

namespace st::core
{
    class Scene::PrivateScene
    {
    public:
        PrivateScene()
        {
        }


        std::vector<Camera> m_cameras;
    };




    /*---------------------*/
	/*-------Public--------*/
	/*---------------------*/
    Scene::Scene(): m_privateScene(std::make_unique<PrivateScene>())
    {
    }

    void Scene::addModel()
    {
    }

    void Scene::addCamera(Camera&& camera)
    {
        m_privateScene->m_cameras.emplace_back(std::move(camera));
    }

    Scene::~Scene() = default;

} // namespace st::core
