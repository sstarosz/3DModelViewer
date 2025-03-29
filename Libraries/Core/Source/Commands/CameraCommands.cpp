#include "Core/Commands/CameraCommands.hpp"

namespace st::core
{

	CreateCameraCommand::CreateCameraCommand(core::ContentManagerHandler contentManager, 
    const float  angleOfView, 
    const float focalLength, 
    const float nearClippingPlane, 
    const float farClippingPlane)
        : m_contentManager(contentManager),
            m_angleOfView(angleOfView),
            m_focalLength(focalLength),
            m_nearClippingPlane(nearClippingPlane),
            m_farClippingPlane(farClippingPlane)
	{
	}

	std::shared_ptr<CameraNode> CreateCameraCommand::execute()
	{
		m_camera = std::make_shared<CameraNode>(m_angleOfView, m_focalLength, m_nearClippingPlane, m_farClippingPlane);
		m_camera->initialize();

		m_transfrom = std::make_shared<TransformNode>();
		m_transfrom->initialize();
		m_transfrom->addChildNode(m_camera);

		m_transfrom->translateBy(Eigen::Vector4f(0.0f, 0.0f, 3.0f, 0.0f));
		m_transfrom->compute();

		core::NodeGraph& nodeGraph = m_contentManager->getMainNodeGraph();
		nodeGraph.addNode(m_camera);

		return m_camera;
	}

	void CreateCameraCommand::undo()
	{
		// TODO implement undo
	}

} // namespace st::core
