#include "Core/DataTypes/Camera.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "Eigen/Dense"

#include <spdlog/spdlog.h>

#include <numbers>
#include <print>

namespace st::core
{

	Camera::Camera(float angleOfView, float focalLength,
				   float nearClippingPlane, float farClippingPlane) :
		m_angleOfView(angleOfView),
		m_focalLength(focalLength),
		m_nearClippingPlane(nearClippingPlane),
		m_farClippingPlane(farClippingPlane),
		m_width(900.0f),
		m_height(600.0f)
	{
		spdlog::info("Camera created");
	}

	Eigen::Matrix4f Camera::getViewMatrix() const
	{
		return Eigen::Inverse(m_cameraTransform);
    }

    Eigen::Matrix4f Camera::getProjectionMatrix() const
	{
		float aspectRatio = m_width / m_height;
		float radAngleOfView = m_angleOfView * std::numbers::pi_v<float> / 180.0f;
		float tanHalfFov = std::tan(radAngleOfView / 2.0f);

		Eigen::Matrix4f projectionMatrix = Eigen::Matrix4f::Zero();
		projectionMatrix(0, 0) = 1.0f / (aspectRatio * tanHalfFov);
		projectionMatrix(1, 1) = 1.0f / tanHalfFov;
		projectionMatrix(2, 2) = m_farClippingPlane / (m_farClippingPlane - m_nearClippingPlane);
		projectionMatrix(2, 3) = -(m_nearClippingPlane * m_farClippingPlane) / (m_farClippingPlane - m_nearClippingPlane);
		projectionMatrix(3, 2) = 1.0f;


		//Since vulkan clip space has inverted Y and half Z, we need to apply a post view rotation
		//Eigen::Matrix4f postViewRotation = Eigen::Matrix4f::Identity();
		//postViewRotation(1, 1) = 1.0f;
		//postViewRotation(2, 2) = 1.0f;

		spdlog::info("!!!Projection Matrix: {}", projectionMatrix);

		return projectionMatrix;
	}

} // namespace st::core

