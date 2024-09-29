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

	Eigen::Matrix4f lookAt(const Eigen::Vector3f& eye, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
	{
		Eigen::Vector3f zAxis = (eye - target).normalized();
		Eigen::Vector3f xAxis = up.cross(zAxis).normalized();
		Eigen::Vector3f yAxis = zAxis.cross(xAxis);

		Eigen::Matrix4f viewMatrix = Eigen::Matrix4f::Identity();
		viewMatrix(0, 0) = xAxis.x();
		viewMatrix(0, 1) = xAxis.y();
		viewMatrix(0, 2) = xAxis.z();
		viewMatrix(0, 3) = -xAxis.dot(eye);

		viewMatrix(1, 0) = yAxis.x();
		viewMatrix(1, 1) = yAxis.y();
		viewMatrix(1, 2) = yAxis.z();
		viewMatrix(1, 3) = -yAxis.dot(eye);

		viewMatrix(2, 0) = zAxis.x();
		viewMatrix(2, 1) = zAxis.y();
		viewMatrix(2, 2) = zAxis.z();
		viewMatrix(2, 3) = -zAxis.dot(eye);

		
		return viewMatrix;
	}

	Eigen::Matrix4f Camera::getViewMatrix() const
	{
		Eigen::Matrix4f viewMatrix = lookAt(m_position, m_target, m_up);

		return viewMatrix;
    }

    Eigen::Matrix4f Camera::getProjectionMatrix() const
	{
		spdlog::info("Camera::getProjectionMatrix(), Width: {}", m_width);
		spdlog::info("Camera::getProjectionMatrix(), Height: {}", m_height);

		spdlog::info("Angle of view: {}", m_angleOfView);
		spdlog::info("Focal length: {}", m_focalLength);
		spdlog::info("Near clipping plane: {}", m_nearClippingPlane);
		spdlog::info("Far clipping plane: {}", m_farClippingPlane);


		float aspectRatio = m_width / m_height;
		spdlog::info("Aspect ratio: {}", aspectRatio);
		float radianOfView = m_angleOfView * std::numbers::pi_v<float> / 180.0f;
		spdlog::info("Radian of view: {}", radianOfView);

		Eigen::Matrix4f projectionMatrix = Eigen::Matrix4f::Zero();
		projectionMatrix(0, 0) = 1.0f / (aspectRatio * std::tan(radianOfView / 2.0f));
		projectionMatrix(1, 1) = 1.0f / std::tan(radianOfView / 2.0f);

		projectionMatrix(2, 2) = (m_farClippingPlane + m_nearClippingPlane) / (m_nearClippingPlane - m_farClippingPlane);
		projectionMatrix(2, 3) = (2.0f * m_farClippingPlane * m_nearClippingPlane) / (m_nearClippingPlane - m_farClippingPlane);
		projectionMatrix(3, 2) = -1.0f;


		return projectionMatrix;
	}


} // namespace st::core

