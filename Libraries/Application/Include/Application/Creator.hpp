#ifndef ST_APPLICATION_CREATOR_HPP
#define ST_APPLICATION_CREATOR_HPP

#include "Core/CommandManager.hpp"
#include "Core/ContentManager.hpp"
#include "Core/Nodes/CameraNode.hpp"
#include "Renderer/Nodes/StandardMaterial.hpp"
#include "Renderer/Renderer.hpp"

namespace st::application
{
	class Creator
	{
	  public:
		Creator(core::ContentManagerHandler contentManager,
				core::CommandManagerHandler commandManager);

		std::weak_ptr<core::Node> camera(const float angleOfView = 45.0f,
										 const float focalLength = 35.0f,
										 const float nearClippingPlane = 0.1f,
										 const float farClippingPlane = 10000.0f);

		std::weak_ptr<core::Node> plane(const float width = 1.0f, const float height = 1.0f);

		std::weak_ptr<renderer::StandardMaterial> standardMaterial();

		std::weak_ptr<renderer::Renderer> renderer(std::weak_ptr<core::Node> camera);

	  private:
		core::ContentManagerHandler m_contentManager;
		core::CommandManagerHandler m_commandManager;
	};

} // namespace st::application

#endif // ST_APPLICATION_CREATOR_HPP