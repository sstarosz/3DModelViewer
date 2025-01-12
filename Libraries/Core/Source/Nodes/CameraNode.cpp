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

        m_camera.setPosition(Eigen::Vector3f(0.0f, 0.0f, 2.0f));

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

    void CameraNode::setPosition(const Eigen::Vector3f & position)
    {
        m_camera.m_position = position;
    }

    void CameraNode::setTarget(const Eigen::Vector3f & target)
    {
        m_camera.m_target = target;
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

	void CameraNode::beginDrag()
	{
        m_camera.dragging = true;
        m_camera.vDown = m_camera.vNow;

	}

    void CameraNode::endDrag()
    {
        m_camera.dragging = false;
        m_camera.qDown = m_camera.qNow;
        m_camera.mDown = m_camera.mNow;
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



    Eigen::Vector3f mouseOnSphere(Eigen::Vector3f mouse, Eigen::Vector3f center, float radius)
    {
        Eigen::Vector3f spherePos;

        spherePos.x() = (mouse.x() - center.x()) / radius;
        spherePos.y() = (mouse.y() - center.y()) / radius;

        float mag = spherePos.x() * spherePos.x() + spherePos.y() * spherePos.y();
        if (mag > 1.0f)
        {
            float scale = 1.0f / std::sqrt(mag);
            spherePos.x() *= scale;
            spherePos.y() *= scale;
            spherePos.z() = 0.0f;
        }
        else
        {
            spherePos.z() = std::sqrt(1.0f - mag);
        }

        return spherePos;
    }

    void quaterionToBallPoints(const Eigen::Quaternionf& q, Eigen::Vector3f& arcFrom, Eigen::Vector3f& arcTo)
    {
        float s = std::sqrt(q.x() * q.x() + q.y() * q.y());
        if(s == 0.0f)
        {
            arcFrom = Eigen::Vector3f(0.0f, 1.0f, 0.0);
        }
        {
            arcFrom = Eigen::Vector3f(-q.y() / s, q.x() / s, 0.0f);
        }

        //TODO is this just quaternion multiplication?
        arcTo.x() = q.w() * arcFrom.x() - q.z() * arcFrom.y();
        arcTo.y() = q.w() * arcFrom.y() + q.z() * arcFrom.x();
        arcTo.z() = q.x() * arcFrom.y() - q.y() * arcFrom.x();
    }

	/*----------------------*/
	/*-------Operators------*/
	/*----------------------*/
	void CameraNode::orbit(float deltaX, float deltaY)
    {
    // Calculate the rotation angle based on the mouse movement
        float width = m_camera.m_width; // Assuming the width of the viewport is 1000 pixels
        float halfWidth = width / 2.0f;
        float rotationAngleY = (deltaX / (halfWidth / 4.0f)) * (std::numbers::pi / 2.0f); // 90 degrees in radians
        float rotationAngleX = (deltaY / (halfWidth / 4.0f)) * (std::numbers::pi / 2.0f); // 90 degrees in radians

        // Create quaternions for the rotations
        Eigen::Quaternionf qYaw(Eigen::AngleAxisf(rotationAngleY, Eigen::Vector3f::UnitY()));
        Eigen::Quaternionf qPitch(Eigen::AngleAxisf(rotationAngleX, Eigen::Vector3f::UnitX()));

        // Calculate the new camera position
        Eigen::Vector3f offset = m_camera.m_position - m_camera.m_target;
        Eigen::Vector3f newOffset = qYaw * qPitch * offset;

        // Update the camera position
        m_camera.m_position = m_camera.m_target + newOffset;

        // Recalculate the up vector to avoid gimbal lock
        Eigen::Vector3f newRelativeTarget = m_camera.m_target - m_camera.m_position;
        Eigen::Vector3f right = newRelativeTarget.cross(m_camera.m_up).normalized();
        m_camera.m_up = right.cross(newRelativeTarget).normalized();
    }

	void CameraNode::pan(float deltaX, float deltaY)
	{
		constexpr float sensitivity = 0.001f;

		Eigen::Vector3f right = (m_camera.m_target - m_camera.m_position).cross(m_camera.m_up).normalized();
		Eigen::Vector3f up = right.cross(m_camera.m_position - m_camera.m_target).normalized();

		m_camera.m_position += -right * deltaX * sensitivity + up * deltaY * sensitivity;
		m_camera.m_target += -right * deltaX * sensitivity + up * deltaY * sensitivity;
	}

	void CameraNode::dolly(float deltaX, float deltaY)
	{
		constexpr float sensitivity = 0.01f;

        Eigen::Vector3f direction = (m_camera.m_target - m_camera.m_position).normalized();
        m_camera.m_position += direction * deltaY * sensitivity;

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