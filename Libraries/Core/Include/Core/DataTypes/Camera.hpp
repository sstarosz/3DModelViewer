#ifndef ST_CORE_CAMERA_HPP
#define ST_CORE_CAMERA_HPP

#include <Eigen/Core>

namespace st::core
{

	class Camera
	{
	  public:
		Camera() = default;
		Camera(float angleOfView, float focalLength,
			   float nearClippingPlane, float farClippingPlane);

		Eigen::Matrix4f getViewMatrix() const;
		Eigen::Matrix4f getProjectionMatrix() const;

		void orbit(float deltaX, float deltaY);
		void pan(float deltaX, float deltaY);
		void zoom(float deltaX, float deltaY);

		// private:
		// Camera location
		Eigen::Vector3f m_position;
		Eigen::Vector3f m_target;
		Eigen::Vector3f m_up;

		// Camera parameters
		float m_angleOfView;
		float m_focalLength;
		float m_nearClippingPlane;
		float m_farClippingPlane;

		// Camera state
		enum class State
		{
			eIdle,
			eOrbit,
			ePan,
			eZoom
		};

		State m_currentState = State::eIdle;
		float m_mouseClickX;
		float m_mouseClickY;

	};

} // namespace st::core

#endif // ST_CORE_CAMERA_HPP