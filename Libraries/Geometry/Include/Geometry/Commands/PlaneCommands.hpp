#ifndef ST_GEOMETRY_PLANE_COMMANDS_HPP
#define ST_GEOMETRY_PLANE_COMMANDS_HPP

#include "Core/Command.hpp"
#include "Core/ContentManager.hpp"
#include "Geometry/Nodes/Plane.hpp"


namespace st::geometry
{
    class CreatePlaneCommand : public core::Command
	{
		public:
		CreatePlaneCommand(core::ContentManagerHandler contentManager);

		void execute() override;
		void undo() override;
		std::shared_ptr<geometry::Plane> getResult();

	  private:
		core::ContentManagerHandler m_contentManager;
		std::shared_ptr<geometry::Plane> m_plane;
	};


} // namespace st::geometry

#endif // ST_GEOMETRY_PLANE_COMMANDS_HPP