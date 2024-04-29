#ifndef ST_CORE_TRANSFORM_HPP
#define ST_CORE_TRANSFORM_HPP


#include "StObject.hpp"

#include <memory>
#include <Eigen/Core>


namespace st::core
{
    class Transform
    {
    public:
        Transform();
        Transform(StObject& object);
        Transform(std::weak_ptr<StObject> object) {}//TODO clean this up

        ~Transform();
        Transform(const Transform&) = delete;
        Transform(Transform&&) = delete;
        Transform& operator=(const Transform&) = delete;
        Transform& operator=(Transform&&) = delete;


        Eigen::Vector3f getTranslation() const;
        void setTranslation(const Eigen::Vector3f& translation);


    private:
        class PrivateTransform;
        std::unique_ptr<PrivateTransform> m_privateTransform;
    };


} // namespace st::core

#endif // ST_CORE_TRANSFORM_HPP