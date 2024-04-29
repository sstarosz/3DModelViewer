#ifndef ST_CORE_SCENE_HPP
#define ST_CORE_SCENE_HPP

#include <memory>
#include <vector>

namespace st::core
{
    class Camera;
    class StObject;
    class Node;
    class NodeGraph;
} // namespace st::core

namespace st::core
{
    class Scene
    {
    friend class SceneContent;

    public:
        Scene();
        ~Scene();
        Scene(const Scene&) = delete;
        Scene(Scene&&) noexcept;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) noexcept;

        void addCamera(Camera&& camera);
        void addNode(Node&& node);

        SceneContent getSceneContent() const;

        //Debug
        void dumpScene() const;

    private:
        class PrivateScene;
        std::unique_ptr<PrivateScene> m_privateScene;
    };

    class SceneContent
    {
    public:
        SceneContent(std::vector<std::shared_ptr<StObject>>::iterator objectIteratorBegin,
                     std::vector<std::shared_ptr<StObject>>::iterator objectIteratorEnd);

        class SceneIterator
        {
        public:
            SceneIterator(std::vector<std::shared_ptr<StObject>>::iterator begin,
                          std::vector<std::shared_ptr<StObject>>::iterator end);

            SceneIterator& operator++();
            bool operator!=(const SceneIterator& other) const;
            std::shared_ptr<StObject>& operator*();

        private:
            std::vector<std::shared_ptr<StObject>>::iterator m_objectIteratorBegin;
            std::vector<std::shared_ptr<StObject>>::iterator m_objectIteratorEnd;
        };

    public:
        SceneIterator begin() const;
        SceneIterator end() const;

    private:
        std::vector<std::shared_ptr<StObject>>::iterator m_objectIteratorBegin;
        std::vector<std::shared_ptr<StObject>>::iterator m_objectIteratorEnd;
    };


} // namespace st::core

#endif // ST_CORE_SCENE_HPP
