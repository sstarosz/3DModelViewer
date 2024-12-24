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
        m_camera.vNow.x() = deltaX;
        m_camera.vNow.y() = deltaY;

        m_camera.vFrom = mouseOnSphere(m_camera.vDown, m_camera.center, m_camera.radius);
        m_camera.vTo = mouseOnSphere(m_camera.vNow, m_camera.center, m_camera.radius);

        std::printf("vFrom: %f, %f, %f\n", m_camera.vFrom.x(), m_camera.vFrom.y(), m_camera.vFrom.z());
        std::printf("vTo: %f, %f, %f\n", m_camera.vTo.x(), m_camera.vTo.y(), m_camera.vTo.z());

        m_camera.qDrag = Eigen::Quaternionf::FromTwoVectors(m_camera.vFrom, m_camera.vTo);
        m_camera.qNow = m_camera.qDrag * m_camera.qDown;

        //Convert a unit quaternion to two points on a unit sphere
        //quaterionToBallPoints(m_camera.qDown, m_camera.vectorRotatedFrom, m_camera.vectorRotatedTo);

        //Quaterion to matrix
        m_camera.mNow =  m_camera.qNow.toRotationMatrix();

        //Transform the camera position
        m_camera.m_position = m_camera.mNow * m_camera.m_position;

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