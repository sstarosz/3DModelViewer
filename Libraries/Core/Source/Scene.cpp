#include "Scene.hpp"

namespace st::core
{
    class Scene::PrivateScene
    {
    public:
        PrivateScene()
        {
        }
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

    void Scene::addCamera(const Camera& camera)
    {
    }

    Scene::~Scene() = default;

} // namespace st::core
