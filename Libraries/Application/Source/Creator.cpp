#include "Creator.hpp"

#include "Geometry/Commands/PlaneCommands.hpp"
#include "Renderer/Commands/RendererCommands.hpp"
#include "Renderer/Commands/StandardMaterialCommands.hpp"

namespace st::application
{
	Creator::Creator(core::ContentManagerHandler contentManager,
					 core::CommandManagerHandler commandManager) :
		m_contentManager(contentManager),
		m_commandManager(commandManager)
	{
	}

	std::weak_ptr<core::Node> Creator::camera(
		const float angleOfView,
		const float focalLength,
		const float nearClippingPlane,
		const float farClippingPlane)
	{
		std::shared_ptr<core::Camera> camera = std::make_shared<core::Camera>(angleOfView,
																			  focalLength,
																			  nearClippingPlane,
																			  farClippingPlane);

		// TODO

		return std::weak_ptr<core::Node>{camera};
	}

	std::weak_ptr<core::Node> Creator::plane(
		[[maybe_unused]] const float width,
		[[maybe_unused]] const float height)
	{
		spdlog::info("Creator::plane()");
		// Create Plane command
		std::unique_ptr<geometry::CreatePlaneCommand> command = std::make_unique<geometry::CreatePlaneCommand>(m_contentManager);
		geometry::CreatePlaneCommand* commandPtr = command.get();
		m_commandManager->execute(std::move(command));

		return std::weak_ptr<core::Node>{commandPtr->getResult()};
	}

	std::weak_ptr<renderer::StandardMaterial> Creator::standardMaterial()
	{
		spdlog::info("Creator::standardMaterial()");
		std::unique_ptr<renderer::CreateStandardMaterialCommand> command = std::make_unique<renderer::CreateStandardMaterialCommand>(m_contentManager);
		renderer::CreateStandardMaterialCommand* commandPtr = command.get();
		m_commandManager->execute(std::move(command));

		return std::weak_ptr<renderer::StandardMaterial>{commandPtr->getResult()};
	}

	std::weak_ptr<renderer::Renderer> Creator::renderer(std::weak_ptr<core::Node> camera)
	{
		spdlog::info("Creator::renderer()");
		std::unique_ptr<renderer::CreateRendererCommand> command = std::make_unique<renderer::CreateRendererCommand>(m_contentManager);
		renderer::CreateRendererCommand* commandPtr = command.get();
		m_commandManager->execute(std::move(command));
		return std::weak_ptr<renderer::Renderer>{commandPtr->getResult()};
	}

} // namespace st::application