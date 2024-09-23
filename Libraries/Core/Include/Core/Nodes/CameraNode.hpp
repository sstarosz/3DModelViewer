#ifndef ST_CORE_NODES_CAMERANODE_HPP
#define ST_CORE_NODES_CAMERANODE_HPP

#include "Core/Node.hpp"
#include "Core/DataTypes/Camera.hpp"

#include <Eigen/Core>

namespace st::core
{

	class CameraNode : public Node //TODO: Make actual node with input and output
	{
	  public:
		CameraNode();
		CameraNode(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane);

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

		/*----------------------*/
		/*-------Operators------*/
		/*----------------------*/
		
		void orbit (float dx, float dy);
		void pan (float dx, float dy);
		void dolly (float dx, float dy);

		Eigen::Matrix4f getViewMatrix() const;
		Eigen::Matrix4f getProjectionMatrix() const;

	  private:
		struct Inputs
		{
			//TODO: Add input attributes
			NumericInputHandler<float> angleOfView;
		} m_input;
		
		struct Outputs
		{
			TypedOutputHandler<core::Camera> camera;
		} m_output;

		Camera m_camera;

	};

} // namespace st::core

#endif // ST_CORE_NODES_CAMERANODE_HPP
