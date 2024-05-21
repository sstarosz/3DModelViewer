#include "Camera.hpp"

namespace st::core
{
    class Camera::PrivateCamera
    {
    public:
        PrivateCamera() :
            m_angleOfView(45.0f),
            m_focalLength(35.0f),
            m_nearClippingPlane(0.1f),
            m_farClippingPlane(10000.0f)
        {
        }

        PrivateCamera(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane) :
            m_angleOfView(angleOfView),
            m_focalLength(focalLength),
            m_nearClippingPlane(nearClippingPlane),
            m_farClippingPlane(farClippingPlane)
        {
        }



    public:
        float m_angleOfView;
        float m_focalLength;

        float m_nearClippingPlane;
        float m_farClippingPlane;
    };



    /*---------------------*/
    /*-------Public--------*/
    /*---------------------*/
    Camera::Camera(): m_privateCamera(std::make_unique<PrivateCamera>())
    {
    }

    Camera::Camera(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane): 
        m_privateCamera(std::make_unique<PrivateCamera>(angleOfView, focalLength, nearClippingPlane, farClippingPlane))
    {
    }

    Camera::~Camera() = default;

    Camera::Camera(Camera&&) noexcept = default;

    Camera& Camera::operator=(Camera&&) = default;

    void Camera::setAngleOfView(float angleOfView)
    {
        m_privateCamera->m_angleOfView = angleOfView;
    }

    void Camera::setFocalLength(float focalLength)
    {
        m_privateCamera->m_focalLength = focalLength;
    }

    void Camera::setNearClippingPlane(float nearClippingPlane)
    {
        m_privateCamera->m_nearClippingPlane = nearClippingPlane;
    }

    void Camera::setFarClippingPlane(float farClippingPlane)
    {
        m_privateCamera->m_farClippingPlane = farClippingPlane;
    }

    float Camera::getAngleOfView() const
    {
        return m_privateCamera->m_angleOfView;
    }

    float Camera::getFocalLength() const
    {
        return m_privateCamera->m_focalLength;
    }

    float Camera::getNearClippingPlane() const
    {
        return m_privateCamera->m_nearClippingPlane;
    }

    float Camera::getFarClippingPlane() const
    {
        return m_privateCamera->m_farClippingPlane;
    }

    
    



} // namespace st::core