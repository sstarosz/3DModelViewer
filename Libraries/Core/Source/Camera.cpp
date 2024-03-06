#include "Camera.hpp"

namespace st::core
{
    class Camera::PrivateCamera
    {
    public:
        PrivateCamera() :
            viewMatrix(Eigen::Matrix4f::Identity()),
            projectionMatrix(Eigen::Matrix4f::Identity()),
            position(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
            target(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
            up(Eigen::Vector3f(0.0f, 0.0f, 0.0f))
        {
        }

        PrivateCamera(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up) :
            viewMatrix(Eigen::Matrix4f::Identity()),
            projectionMatrix(Eigen::Matrix4f::Identity()),
            position(position),
            target(target),
            up(up)
        {
        }



    public:
        Eigen::Matrix4f viewMatrix;
        Eigen::Matrix4f projectionMatrix;

        Eigen::Vector3f position;
        Eigen::Vector3f target;
        Eigen::Vector3f up;
    };



    /*---------------------*/
    /*-------Public--------*/
    /*---------------------*/
    Camera::Camera(): m_privateCamera(std::make_unique<PrivateCamera>())
    {
    }

    Camera::Camera(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up): 
        m_privateCamera(std::make_unique<PrivateCamera>())
    {
        m_privateCamera->position = position;
        m_privateCamera->target = target;
        m_privateCamera->up = up;
    }

    void Camera::setViewMatrix(const Eigen::Matrix4f& viewMatrix)
    {
        m_privateCamera->viewMatrix = viewMatrix;
    }

    void Camera::setProjectionMatrix(const Eigen::Matrix4f& projectionMatrix)
    {
        m_privateCamera->projectionMatrix = projectionMatrix;
    }

    const Eigen::Matrix4f Camera::getViewMatrix() const
    {
        return m_privateCamera->viewMatrix;
    }

    const Eigen::Matrix4f Camera::getProjectionMatrix() const
    {
        return m_privateCamera->projectionMatrix;
    }

    Camera::~Camera() = default;

} // namespace st::core