#include "Scene.hpp"
#include "Camera.hpp"
#include "StObject.hpp"
#include "EventSystem.hpp"
#include "NodeGraph.hpp"

#include <vector>
#include <memory>

namespace st::core
{
    /*---------------------*/
    /*-------Private-------*/
    /*---------------------*/
    class Scene::PrivateScene
    {
    public:
        PrivateScene()
        {
        }


        std::vector<Camera> m_cameras;
        std::vector<std::shared_ptr<StObject>> m_objects;
        NodeGraph m_nodeGraph;
    };




    /*---------------------*/
	/*-------Public--------*/
	/*---------------------*/
    Scene::Scene(): m_privateScene(std::make_unique<PrivateScene>())
    {
    }

    void Scene::addCamera(Camera&& camera)
    {
        m_privateScene->m_cameras.emplace_back(std::move(camera));
    }

    void Scene::addNode(Node&& node)
    {
        auto* newNode = m_privateScene->m_nodeGraph.addNode(std::move(node));

        EventData event;
        event.m_eventType = Event::Type::eAddedModel;
        event.m_eventData = newNode;
        EventSystem::sendEvent(event);
    }

    SceneContent Scene::getSceneContent() const
    {
        return SceneContent(m_privateScene->m_objects.begin(), m_privateScene->m_objects.end());

    }

	void Scene::dumpScene() const
	{
        //m_nodeGraph
	}

	Scene::~Scene() = default;

    Scene::Scene(Scene&&) noexcept = default;

    Scene& Scene::operator=(Scene&&) noexcept = default;

    /*---------------------------*/
    /*-------Scene Content-------*/
    /*---------------------------*/
    SceneContent::SceneContent(std::vector<std::shared_ptr<StObject>>::iterator objectIteratorBegin,
                               std::vector<std::shared_ptr<StObject>>::iterator objectIteratorEnd):
        m_objectIteratorBegin(objectIteratorBegin),
        m_objectIteratorEnd(objectIteratorEnd)
    {
    }


    SceneContent::SceneIterator::SceneIterator(std::vector<std::shared_ptr<StObject>>::iterator objectIteratorBegin,
                                               std::vector<std::shared_ptr<StObject>>::iterator objectIteratorEnd):
        m_objectIteratorBegin(objectIteratorBegin),
        m_objectIteratorEnd(objectIteratorBegin)
    {
    }

    SceneContent::SceneIterator& SceneContent::SceneIterator::operator++()
    {
        ++m_objectIteratorBegin;
        return *this;
    }

    bool SceneContent::SceneIterator::operator!=(const SceneIterator& other) const
    {
        return m_objectIteratorBegin != other.m_objectIteratorBegin;
    }

    std::shared_ptr<StObject>& SceneContent::SceneIterator::operator*()
    {
        return *m_objectIteratorBegin;
    }

    SceneContent::SceneIterator SceneContent::begin() const
    {
        return SceneIterator(m_objectIteratorBegin, m_objectIteratorEnd);
    }

    SceneContent::SceneIterator SceneContent::end() const
    {
        return SceneIterator(m_objectIteratorEnd, m_objectIteratorEnd);
    }

} // namespace st::core
