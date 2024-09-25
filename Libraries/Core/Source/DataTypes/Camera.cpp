#include "Core/DataTypes/Camera.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "Eigen/Dense"

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
		Eigen::Matrix4f viewMatrix = Eigen::Matrix4f::Identity();

		Eigen::Vector3f forward = (m_target - m_position).normalized();
		Eigen::Vector3f right = forward.cross(m_up).normalized();
		Eigen::Vector3f up = right.cross(forward).normalized();

		viewMatrix(0, 0) = right.x();
		viewMatrix(1, 0) = right.y();
		viewMatrix(2, 0) = right.z();
		viewMatrix(3, 0) = -right.dot(m_position);

		viewMatrix(0, 1) = up.x();
		viewMatrix(1, 1) = up.y();
		viewMatrix(2, 1) = up.z();
		viewMatrix(3, 1) = -up.dot(m_position);

		viewMatrix(0, 2) = -forward.x();
		viewMatrix(1, 2) = -forward.y();
		viewMatrix(2, 2) = -forward.z();
		viewMatrix(3, 2) = forward.dot(m_position);

		return viewMatrix;
    }

    Eigen::Matrix4f Camera::getProjectionMatrix() const
	{
		Eigen::Matrix4f projectionMatrix = Eigen::Matrix4f::Zero();

		float aspectRatio = 1.0f;
		float top = tan(m_angleOfView / 2.0f) * m_nearClippingPlane;
		float right = aspectRatio * top;

		projectionMatrix(0, 0) = m_nearClippingPlane / right;
		projectionMatrix(1, 1) = m_nearClippingPlane / top;
		projectionMatrix(2, 2) = -(m_farClippingPlane + m_nearClippingPlane) / (m_farClippingPlane - m_nearClippingPlane);
		projectionMatrix(2, 3) = -2.0f * m_farClippingPlane * m_nearClippingPlane / (m_farClippingPlane - m_nearClippingPlane);
		projectionMatrix(3, 2) = -1.0f;

		return projectionMatrix;
	}


} // namespace st::core

