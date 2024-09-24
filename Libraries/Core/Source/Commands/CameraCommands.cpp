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

	void CreateCameraCommand::execute()
	{
		m_camera = std::make_shared<CameraNode>();
		m_camera->initialize();

		core::NodeGraph& nodeGraph = m_contentManager->getMainNodeGraph();
		nodeGraph.addNode(m_camera);
	}

	void CreateCameraCommand::undo()
	{
		// TODO implement undo
	}

	std::shared_ptr<CameraNode> CreateCameraCommand::getResult()
	{
		return m_camera;
	}
} // namespace st::core
