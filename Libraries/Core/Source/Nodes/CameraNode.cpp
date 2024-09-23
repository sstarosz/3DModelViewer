#include "CameraNode.hpp"

namespace st::core
{
    /*----------------------*/
    /*------Constructor-----*/
    /*----------------------*/
    CameraNode::CameraNode() : //TODO: Add default values
        Node(),
        m_camera(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    CameraNode::CameraNode(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane) :
        Node(),
        m_camera(angleOfView, focalLength, nearClippingPlane, farClippingPlane)
    {
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


        return true;
    }

    bool CameraNode::compute()
    {



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
    
} // namespace st::core