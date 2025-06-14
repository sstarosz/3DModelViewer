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

	std::shared_ptr<core::Node> Creator::camera(
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
		return result;
	}

	std::shared_ptr<core::Node> Creator::plane(
		[[maybe_unused]] const float width,
		[[maybe_unused]] const float height)
	{
		spdlog::info("Creator::plane()");
		std::unique_ptr<geometry::CreatePlaneCommand> command = std::make_unique<geometry::CreatePlaneCommand>(m_contentManager);
		auto result = m_commandManager->execute(std::move(command));
		return result;
	}

	std::shared_ptr<renderer::StandardMaterial> Creator::standardMaterial()
	{
		spdlog::info("Creator::standardMaterial()");
		std::unique_ptr<renderer::CreateStandardMaterialCommand> command = std::make_unique<renderer::CreateStandardMaterialCommand>(m_contentManager);
		auto result = m_commandManager->execute(std::move(command));
		return result;
	}

	std::shared_ptr<renderer::Renderer> Creator::renderer(std::shared_ptr<core::Node> camera)
	{
		spdlog::info("Creator::renderer()");
		std::unique_ptr<renderer::CreateRendererCommand> command = std::make_unique<renderer::CreateRendererCommand>(m_contentManager, camera);
		auto result = m_commandManager->execute(std::move(command));
		return result;
	}

} // namespace st::application