#include "Transform.hpp"

namespace st::core 
{
    //class Transform::PrivateTransform
    //{
    //public:
    //    PrivateTransform()
    //    {
    //    }
    //};

    /*---------------------*/
    /*-------Public--------*/
    /*---------------------*/
    Transform::Transform()//: m_privateTransform(std::make_unique<PrivateTransform>())
    {
    }

    Transform::Transform(StObject& /*object*/)//: m_privateTransform(std::make_unique<PrivateTransform>())
    {
    }

    Eigen::Vector3f Transform::getTranslation() const
    {
        return Eigen::Vector3f();
    }

    void Transform::setTranslation(const Eigen::Vector3f& /*translation*/)
    {
    }


    Transform::~Transform() = default;

    Transform::Transform(Transform&&) = default;

    Transform& Transform::operator=(Transform&&) = default;


} // namespace st::core