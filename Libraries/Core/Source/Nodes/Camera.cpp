#include "Camera.hpp"

namespace st::core
{
    /*----------------------*/
    /*------Constructor-----*/
    /*----------------------*/
    Camera::Camera() : //TODO: Add default values
        Node(),
        m_angleOfView(0.0f),
        m_focalLength(0.0f),
        m_nearClippingPlane(0.0f),
        m_farClippingPlane(0.0f)
    {
    }

    Camera::Camera(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane) :
        Node(),
        m_angleOfView(angleOfView),
        m_focalLength(focalLength),
        m_nearClippingPlane(nearClippingPlane),
        m_farClippingPlane(farClippingPlane)
    {
    }

    /*----------------------*/
    /*-------Methods--------*/
    /*----------------------*/
    bool Camera::initialize() 
    {
        return true;
    }

    bool Camera::compute()
    {
        return true;
    }

    /*----------------------*/
    /*-------Setters--------*/
    /*----------------------*/
    void Camera::setAngleOfView(float angleOfView)
    {
        m_angleOfView = angleOfView;
    }

    void Camera::setFocalLength(float focalLength)
    {
        m_focalLength = focalLength;
    }

    void Camera::setNearClippingPlane(float nearClippingPlane)
    {
        m_nearClippingPlane = nearClippingPlane;
    }

    void Camera::setFarClippingPlane(float farClippingPlane)
    {
        m_farClippingPlane = farClippingPlane;
    }

    /*----------------------*/
    /*-------Getters--------*/
    /*----------------------*/
    float Camera::getAngleOfView() const
    {
        return m_angleOfView;
    }

    float Camera::getFocalLength() const
    {
        return m_focalLength;
    }

    float Camera::getNearClippingPlane() const
    {
        return m_nearClippingPlane;
    }

    float Camera::getFarClippingPlane() const
    {
        return m_farClippingPlane;
    }
    
} // namespace st::core