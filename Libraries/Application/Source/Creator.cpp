#include "Creator.hpp"

#include "Core/Commands/CameraCommands.hpp"
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
		spdlog::info("Creator::camera()");
		std::unique_ptr<core::CreateCameraCommand> command = std::make_unique<core::CreateCameraCommand>(m_contentManager,
																											angleOfView,
																											focalLength,
																											nearClippingPlane,
																											farClippingPlane);
		auto result = m_commandManager->execute(std::move(command));

		return std::weak_ptr<core::Node>{result};
	}

	std::weak_ptr<core::Node> Creator::plane(
		[[maybe_unused]] const float width,
		[[maybe_unused]] const float height)
	{
		spdlog::info("Creator::plane()");
		// Create Plane command
		std::unique_ptr<geometry::CreatePlaneCommand> command = std::make_unique<geometry::CreatePlaneCommand>(m_contentManager);
		geometry::CreatePlaneCommand* commandPtr = command.get();
		
		geometry::CreatePlaneCommand::ResultType result = m_commandManager->execute(std::move(command));

		return std::weak_ptr<core::Node>{result};
	}

	std::weak_ptr<renderer::StandardMaterial> Creator::standardMaterial()
	{
		spdlog::info("Creator::standardMaterial()");
		std::unique_ptr<renderer::CreateStandardMaterialCommand> command = std::make_unique<renderer::CreateStandardMaterialCommand>(m_contentManager);
		renderer::CreateStandardMaterialCommand* commandPtr = command.get();
		
		auto result = m_commandManager->execute(std::move(command));

		return std::weak_ptr<renderer::StandardMaterial>{result};
	}

	std::weak_ptr<renderer::Renderer> Creator::renderer(std::weak_ptr<core::Node> camera)
	{
		spdlog::info("Creator::renderer()");
		std::unique_ptr<renderer::CreateRendererCommand> command = std::make_unique<renderer::CreateRendererCommand>(m_contentManager, camera.lock());
		renderer::CreateRendererCommand* commandPtr = command.get();
		auto result = m_commandManager->execute(std::move(command));
		
		return std::weak_ptr<renderer::Renderer>{result};
	}

} // namespace st::application