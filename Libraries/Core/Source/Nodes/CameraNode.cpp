#include "CameraNode.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#include <numbers>
#include <string>

namespace st::core
{
    /*----------------------*/
    /*------Constructor-----*/
    /*----------------------*/
    CameraNode::CameraNode() : //TODO: Add default values
        Node(),
        m_camera(0.0f, 0.0f, 0.0f, 0.0f)
    {
        spdlog::info("CameraNode: Default constructor");
    }

    CameraNode::CameraNode(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane) :
        Node(),
        m_camera(angleOfView, focalLength, nearClippingPlane, farClippingPlane)
    {
        spdlog::info("CameraNode: Constructor with parameters");
    }

    /*----------------------*/
    /*-------Methods--------*/
    /*----------------------*/
    bool CameraNode::initialize() 
    {
        m_input.angleOfView = NumericAttribute<float>::Builder("Angle of View", 45.0F).setReadable(true).build();
        addAttribute(m_input.angleOfView);

        m_output.camera = TypedAttribute<Camera>::Builder("Camera").setReadable(true).build();
        addAttribute(m_output.camera);

        defineNode("Camera");

        return true;
    }

    bool CameraNode::compute()
    {
        spdlog::warn("CameraNode::compute()");
        //m_camera.


        m_camera.m_cameraTransform = getInclusiveMatrix();
        spdlog::warn("CameraNode::compute() - Camera transform: {}", m_camera.m_cameraTransform);

        m_output.camera = m_camera;
        return true;
    }

    /*----------------------*/
    /*-------Setters--------*/
    /*----------------------*/
    void CameraNode::setAngleOfView(float angleOfView)
    {
        m_camera.m_angleOfView = angleOfView;
    }

    void CameraNode::setFocalLength(float focalLength)
    {
        m_camera.m_focalLength = focalLength;
    }

    void CameraNode::setNearClippingPlane(float nearClippingPlane)
    {
        m_camera.m_nearClippingPlane = nearClippingPlane;
    }

    void CameraNode::setFarClippingPlane(float farClippingPlane)
    {
        m_camera.m_farClippingPlane = farClippingPlane;
    }

	void CameraNode::setWidth(float width)
	{
        spdlog::info("CameraNode::setWidth(), width: {}", width);
        m_camera.m_width = width;
	}

    void CameraNode::setHeight(float height)
    {
        spdlog::info("CameraNode::setHeight(), height: {}", height);
        m_camera.m_height = height;
    }

	/*----------------------*/
    /*-------Getters--------*/
    /*----------------------*/
    float CameraNode::getAngleOfView() const
    {
        return m_camera.m_angleOfView;
    }

    float CameraNode::getFocalLength() const
    {
        return m_camera.m_focalLength;
    }

    float CameraNode::getNearClippingPlane() const
    {
        return m_camera.m_nearClippingPlane;
    }

    float CameraNode::getFarClippingPlane() const
    {
        return m_camera.m_farClippingPlane;
    }

	Eigen::Matrix4f CameraNode::getViewMatrix() const
	{
		return m_camera.getViewMatrix();
	}

	Eigen::Matrix4f CameraNode::getProjectionMatrix() const
	{
		return m_camera.getProjectionMatrix();
	}

} // namespace st::core