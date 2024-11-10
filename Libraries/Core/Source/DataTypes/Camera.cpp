#include "Core/DataTypes/Camera.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "Eigen/Dense"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>

#include <numbers>
#include <print>

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
		//Eigen::Vector3f forward = (m_target - m_position).normalized();
//
		//Eigen::Vector3f right = m_up.cross(forward).normalized();
//
		//Eigen::Vector3f up = forward.cross(right);
//
		//Eigen::Matrix4f viewMatrix = Eigen::Matrix4f::Identity();
//
		//viewMatrix(0, 0) = right.x();
		//viewMatrix(0, 1) = right.y();
		//viewMatrix(0, 2) = right.z();
		//viewMatrix(0, 3) = -right.dot(m_position);
//
		//viewMatrix(1, 0) = up.x();
		//viewMatrix(1, 1) = up.y();
		//viewMatrix(1, 2) = up.z();
		//viewMatrix(1, 3) = -up.dot(m_position);
//
		//viewMatrix(2, 0) = -forward.x();
		//viewMatrix(2, 1) = -forward.y();
		//viewMatrix(2, 2) = -forward.z();
		//viewMatrix(2, 3) = forward.dot(m_position);
//
		//viewMatrix(3, 0) = 0.0f;
		//viewMatrix(3, 1) = 0.0f;
		//viewMatrix(3, 2) = 0.0f;
		//viewMatrix(3, 3) = 1.0f;
		spdlog::info("Camera position: {}", m_position);
		spdlog::info("Camera target: {}", m_target);
		spdlog::info("Camera up: {}", m_up);

		glm::mat4 viewMatrixGlm = glm::lookAt(glm::vec3(m_position.x(), m_position.y(), m_position.z()),
											glm::vec3(m_target.x(), m_target.y(), m_target.z()),
											glm::vec3(m_up.x(), m_up.y(), m_up.z()));

		Eigen::Matrix4f viewMatrix = Eigen::Matrix4f::Zero();

		for(int row = 0; row < 4; row++)
		{
			for(int col = 0; col < 4; col++)
			{
				viewMatrix(row, col) = viewMatrixGlm[col][row];
			}
		}

		return viewMatrix;
    }

    Eigen::Matrix4f Camera::getProjectionMatrix() const
	{
		spdlog::info("Camera width: {}", m_width);
		spdlog::info("Camera height: {}", m_height);
		spdlog::info("Camera angle of view: {}", m_angleOfView);
		float aspectRatio = m_width / m_height;
		float radAngleOfView = m_angleOfView * std::numbers::pi_v<float> / 180.0f;
		float tanHalfFov = std::tan(radAngleOfView / 2.0f);
		spdlog::info("Aspect Ratio: {}", aspectRatio);
		spdlog::info("Tan Half Fov: {}", tanHalfFov);
		glm::mat4 projectionMatrixGlm = glm::perspectiveRH_ZO(glm::radians(m_angleOfView), aspectRatio, m_nearClippingPlane, m_farClippingPlane);

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

		//projectionMatrix = projectionMatrix * postViewRotation;
		spdlog::info("!!!Projection Matrix: {}", projectionMatrix);

		return projectionMatrix;
	}


} // namespace st::core

