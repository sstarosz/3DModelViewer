#ifndef ST_CORE_CAMERA_HPP
#define ST_CORE_CAMERA_HPP

#include "Core/Node.hpp"

namespace st::core
{

	class Camera : public Node //TODO: Make actual node with input and output
	{
	  public:
		Camera();
		Camera(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane);

		/*----------------------*/
		/*-------Methods--------*/
		/*----------------------*/
		virtual bool initialize() override;
		virtual bool compute() override;

		/*----------------------*/
		/*-------Setters--------*/
		/*----------------------*/
		void setAngleOfView(float angleOfView);
		void setFocalLength(float focalLength);
		void setNearClippingPlane(float nearClippingPlane);
		void setFarClippingPlane(float farClippingPlane);

		/*----------------------*/
		/*-------Getters--------*/
		/*----------------------*/
		float getAngleOfView() const;
		float getFocalLength() const;
		float getNearClippingPlane() const;
		float getFarClippingPlane() const;

	  private:
		float m_angleOfView;
		float m_focalLength;
		float m_nearClippingPlane;
		float m_farClippingPlane;
	};

} // namespace st::core

#endif // ST_CORE_CAMERA_HPP
