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

	void Camera::orbit(float deltaX, float deltaY)
	{
		constexpr float sensitivity = 0.01f;
		
		float theta = std::atan2(m_position.z() - m_target.z(), m_position.x() - m_target.x());
		float phi = std::acos((m_position.y() - m_target.y()) / m_position.norm()); //Or radius

		theta += deltaX * sensitivity;
		phi += deltaY * sensitivity;

		phi = std::clamp(phi, 0.1f, 3.13f); //TODO remove magic numbers

		// Convert Spherical to Cartesian coordinates.
		m_position.x() = m_target.x() + m_focalLength * std::sin(phi) * std::cos(theta);
		m_position.y() = m_target.y() + m_focalLength * std::cos(phi);
		m_position.z() = m_target.z() + m_focalLength * std::sin(phi) * std::sin(theta);
		
	}


} // namespace st::core

