#include "CameraNode.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

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

        m_camera.setPosition(Eigen::Vector3f(1.0f, 0.0f, 0.0f));

        defineNode("Camera");

        return true;
    }

    bool CameraNode::compute()
    {
        spdlog::info("CameraNode::compute()");
        //m_camera.


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

	void CameraNode::setCameraCurrentState(Camera::State state)
	{
        m_camera.m_currentState = state;
	}

	Camera::State CameraNode::getCameraCurrentState() const
	{
		return m_camera.m_currentState;
	}

	void CameraNode::setClickPosition(float x, float y)
	{
        m_camera.m_mouseClickX = x;
        m_camera.m_mouseClickY = y;
	}

	void CameraNode::getClickPosition(float& x, float& y) const
	{
        x = m_camera.m_mouseClickX;
        y = m_camera.m_mouseClickY;
	}

	/*----------------------*/
	/*-------Operators------*/
	/*----------------------*/
	void CameraNode::orbit(float deltaX, float deltaY)
	{
		constexpr float sensitivity = 0.01f;

		Eigen::Vector3f right = (m_camera.m_position - m_camera.m_target).cross(m_camera.m_up).normalized();
		Eigen::Vector3f up = right.cross(m_camera.m_position - m_camera.m_target).normalized();

		m_camera.m_position += -right * deltaX * sensitivity + up * deltaY * sensitivity;
	}
		
	void CameraNode::pan(float deltaX, float deltaY)
	{
		constexpr float sensitivity = 0.01f;

		Eigen::Vector3f right = (m_camera.m_position - m_camera.m_target).cross(m_camera.m_up).normalized();
		Eigen::Vector3f up = right.cross(m_camera.m_position - m_camera.m_target).normalized();

		m_camera.m_position += -right * deltaX * sensitivity + up * deltaY * sensitivity;
		m_camera.m_target += -right * deltaX * sensitivity + up * deltaY * sensitivity;
	}

	void CameraNode::dolly(float deltaX, float deltaY)
	{
		constexpr float sensitivity = 0.01f;

		m_camera.m_focalLength += deltaY * sensitivity;
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