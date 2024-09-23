#include "Core/DataTypes/Camera.hpp"

namespace st::core
{

	Camera::Camera(float angleOfView, float focalLength,
				   float nearClippingPlane, float farClippingPlane) :
		m_angleOfView(angleOfView),
		m_focalLength(focalLength),
		m_nearClippingPlane(nearClippingPlane),
		m_farClippingPlane(farClippingPlane)
	{
	}

	Eigen::Matrix4f Camera::getViewMatrix() const
    {
        return Eigen::Matrix4f();
    }

    Eigen::Matrix4f Camera::getProjectionMatrix() const
    {
        return Eigen::Matrix4f();
    }

	void Camera::orbit(float x, float y)
	{
		//m_currentState = action;
		m_mouseClickX = x;
		m_mouseClickY = y;
	}


} // namespace st::core

