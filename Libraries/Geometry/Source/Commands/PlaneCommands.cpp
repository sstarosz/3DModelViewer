#include "PlaneCommands.hpp"

namespace st::geometry
{

	CreatePlaneCommand::CreatePlaneCommand(core::ContentManagerHandler contentManager) :
		m_contentManager(contentManager)
	{
	}

	std::shared_ptr<geometry::Plane> CreatePlaneCommand::execute()
	{
		m_plane = std::make_shared<geometry::Plane>();
		m_plane->initialize();
		m_contentManager->getMainNodeGraph().addNode(m_plane);

		return m_plane;
	}

	void CreatePlaneCommand::undo()
	{
		// TODO implement
		// m_contentManager.remove(m
	}

} // namespace st::geometry