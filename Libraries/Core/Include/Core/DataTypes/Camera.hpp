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

		void orbit(float x, float y);
		void pan(float x, float y);
		void zoom(float x, float y);

		// private:
		float m_angleOfView;
		float m_focalLength;
		float m_nearClippingPlane;
		float m_farClippingPlane;

		// Camera state
		enum class State
		{
			IDLE,
			ORBIT,
			PAN,
			ZOOM
		};

		State m_currentState = State::IDLE;
		float m_mouseClickX;
		float m_mouseClickY;

	};

} // namespace st::core

#endif // ST_CORE_CAMERA_HPP