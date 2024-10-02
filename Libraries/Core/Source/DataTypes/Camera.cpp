#include "Core/DataTypes/Camera.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "Eigen/Dense"

#include <spdlog/spdlog.h>

#include <numbers>

namespace st::core
{

	Camera::Camera(float angleOfView, float focalLength,
				   float nearClippingPlane, float farClippingPlane) :
		m_position(Eigen::Vector3f::Zero()),
		m_target(Eigen::Vector3f::Zero()),
		m_up(Eigen::Vector3f::UnitY()),
		m_angleOfView(angleOfView),
		m_focalLength(focalLength),
		m_nearClippingPlane(nearClippingPlane),
		m_farClippingPlane(farClippingPlane),
		m_width(900.0f),
		m_height(600.0f),
		m_currentState(State::eIdle),
		m_mouseClickX(0.0f),
		m_mouseClickY(0.0f)
	{
		spdlog::info("Camera created");
	}

	void Camera::setPosition(const Eigen::Vector3f& position)
	{
		m_position = position;
	}



	Eigen::Matrix4f Camera::getViewMatrix() const
	{
		Eigen::Vector3f forward = (m_position - m_target).normalized();

		Eigen::Vector3f right = m_up.cross(forward).normalized();

		Eigen::Vector3f up = forward.cross(right);

		Eigen::Matrix4f viewMatrix = Eigen::Matrix4f::Identity();

		viewMatrix(0, 0) = right.x();
		viewMatrix(0, 1) = right.y();
		viewMatrix(0, 2) = right.z();
		viewMatrix(0, 3) = -right.dot(m_position);

		viewMatrix(1, 0) = up.x();
		viewMatrix(1, 1) = up.y();
		viewMatrix(1, 2) = up.z();
		viewMatrix(1, 3) = -up.dot(m_position);

		viewMatrix(2, 0) = -forward.x();
		viewMatrix(2, 1) = -forward.y();
		viewMatrix(2, 2) = -forward.z();
		viewMatrix(2, 3) = forward.dot(m_position);

		viewMatrix(3, 0) = 0.0f;
		viewMatrix(3, 1) = 0.0f;
		viewMatrix(3, 2) = 0.0f;
		viewMatrix(3, 3) = 1.0f;


		return viewMatrix;
    }

    Eigen::Matrix4f Camera::getProjectionMatrix() const
	{
		float tanHalfFov = std::tan(m_angleOfView / 2.0f);

		Eigen::Matrix4f projectionMatrix = Eigen::Matrix4f::Zero();
		projectionMatrix(0, 0) = 1.0f / (tanHalfFov * m_width / m_height);
		projectionMatrix(1, 1) = 1.0f / tanHalfFov;
		projectionMatrix(2, 2) = -(m_farClippingPlane + m_nearClippingPlane) / (m_farClippingPlane - m_nearClippingPlane);
		projectionMatrix(2, 3) = -(2.0f * m_farClippingPlane * m_nearClippingPlane) / (m_farClippingPlane - m_nearClippingPlane);
		projectionMatrix(3, 2) = -1.0f;
		projectionMatrix(3, 3) = 1.0f;

		return projectionMatrix;
	}


} // namespace st::core

