#include "Viewport.hpp"
#include <QHBoxLayout>
#include <QTimer>
#include <QVulkanInstance>
#include <QWindow>
#include <QLabel>
#include <QMouseEvent>

#include "Renderer/Renderer.hpp"

#include "Core/Nodes/CameraNode.hpp"

namespace st::ui
{

	class FallBackWidget : public QWidget
	{
	  public:
		FallBackWidget(QWidget* parent = nullptr) : QWidget(parent)
		{
			QLabel* label = new QLabel("Renderer Node not found", this);
			label->setAlignment(Qt::AlignCenter);
			label->setStyleSheet("QLabel { background-color : white; color : black; }");

			QHBoxLayout* layout = new QHBoxLayout(this);
			layout->addWidget(label);
			
		}
	};


	/*---------------------*/
	/*-------Private-------*/
	/*---------------------*/
	class Viewport::PrivateWindow : public QWindow
	{
	  public:
		PrivateWindow(core::ContentManagerHandler contentManager,
					  QWindow* parent = nullptr) :
			QWindow(parent),
			m_contentManager(contentManager),
			m_timer(this),
			m_renderer(nullptr),
			m_fallbackLabel(nullptr)
		{
			setSurfaceType(QSurface::VulkanSurface);
			setMinimumSize(QSize(1, 1));
		}

		void showEvent(QShowEvent* event) override
		{
			QWindow::showEvent(event);

			if (!m_initialized)
			{
				// Find Renderer Node inside the content manager
				core::NodeGraph& nodeGraph = m_contentManager->getMainNodeGraph();
				for (auto node : nodeGraph.getNodes())
				{
					if (std::shared_ptr<renderer::Renderer> renderable = std::dynamic_pointer_cast<renderer::Renderer>(node))
					{
						spdlog::info("Renderer Node found");
						m_renderer = renderable;
					}
				}

				if(m_renderer == nullptr)
				{
					spdlog::error("Renderer Node not found");
					return;
				}

				for(auto node : nodeGraph.getNodes())
				{
					if(std::shared_ptr<core::CameraNode> camera = std::dynamic_pointer_cast<core::CameraNode>(node))
					{
						spdlog::info("Camera Node found");
						m_camera = camera;
					}
				}

				if(m_camera == nullptr)
				{
					spdlog::error("Camera Node not found");
					return;
				}
				


				// Create Vulkan Instance for QVulkanWindow
				QVulkanInstance vulkanInstance;
				vulkanInstance.setVkInstance(m_renderer->getVulkanInstance());
				if (!vulkanInstance.create())
				{
					qDebug() << "Failed to create vulkan instance";
					return;
				}
				setVulkanInstance(&vulkanInstance);

				// Embed Surface to Renderer
				m_renderer->setSurface(vulkanInstance.surfaceForWindow(this));

				connect(&m_timer, &QTimer::timeout, this, &PrivateWindow::update);
				m_timer.start(16); // 60fps

				m_initialized = true;

			}
		}

		void resizeEvent(QResizeEvent* event) override
		{
			if (!m_initialized)
				return;

			QWindow::resizeEvent(event);
			m_renderer->changeSwapchainExtent(width(), height());
		}

		void setRenderer(std::shared_ptr<renderer::Renderer> renderer)
		{
			m_renderer = renderer;
		}

		/*----------------------*/
		/*----Event Handlers----*/
		/*----------------------*/
		void mousePressEvent(QMouseEvent* event) override
		{
			spdlog::info("Mouse Pressed");

			auto pos = event->position();

			if(event->modifiers() & Qt::AltModifier)
			{
				spdlog::info("Alt Modifier Pressed");
				if(event->button() == Qt::LeftButton)
				{
					spdlog::info("Alt + Left Button Pressed");
					m_camera->setCameraCurrentState(core::Camera::State::eOrbit);
				}

				if(event->button() == Qt::MiddleButton)
				{
					spdlog::info("Alt + Middle Button Pressed");
					m_camera->setCameraCurrentState(core::Camera::State::ePan);
				}

				if(event->button() == Qt::RightButton)
				{
					spdlog::info("Alt + Right Button Pressed");
					m_camera->setCameraCurrentState(core::Camera::State::eZoom);
				}

				m_camera->setClickPosition(pos.x(), pos.y());
			}
		}

		void mouseMoveEvent(QMouseEvent* event) override
		{
			spdlog::info("Mouse Moved");
			auto pos = event->position();

			float cameraX{0.0f};
			float cameraY{0.0f};

			m_camera->getClickPosition(cameraX, cameraY);

			float deltaX = pos.x() - cameraX;
			float deltaY = pos.y() - cameraY;

			
			if(m_camera->getCameraCurrentState() == core::Camera::State::eOrbit)
			{
				m_camera->orbit(deltaX, deltaY);
			}

			if(m_camera->getCameraCurrentState() == core::Camera::State::ePan)
			{
				m_camera->pan(deltaX, deltaY);
			}

			if(m_camera->getCameraCurrentState() == core::Camera::State::eZoom)
			{
				m_camera->dolly(deltaX, deltaY);
			}

			m_camera->setClickPosition(pos.x(), pos.y());
		}

		void mouseReleaseEvent(QMouseEvent* event) override
		{
			spdlog::info("Mouse Released");

			auto pos = event->position();

			m_camera->setCameraCurrentState(core::Camera::State::eIdle);
		}



	  private:
		// Main loop
		void update()
		{
			if (m_initialized)
			{
				m_renderer->render();
				requestUpdate();
			}
		}

		core::ContentManagerHandler m_contentManager;
		bool m_initialized{false};
		QTimer m_timer;
		std::shared_ptr<renderer::Renderer> m_renderer;
		std::shared_ptr<core::CameraNode> m_camera; //Current active camera
		QLabel* m_fallbackLabel{nullptr};
	};

	/*---------------------*/
	/*-------Public--------*/
	/*---------------------*/
	Viewport::Viewport(
		core::ContentManagerHandler contentManager,
		QWidget* parent,
		Qt::WindowFlags flags) :
		QWidget(parent, flags),
		m_contentManager(contentManager),
		m_window(nullptr)
	{
	}

	void Viewport::initialize()
	{
		QWidget* renderWidget{nullptr};

		if(isRendererSpecified())
		{
			spdlog::info("Renderer Node found");
			m_window = new PrivateWindow(m_contentManager);
			renderWidget = QWidget::createWindowContainer(m_window, this);
		}
		else
		{
			spdlog::error("Renderer Node not found");
			renderWidget = new FallBackWidget(this);
		}

		QHBoxLayout* layout = new QHBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(renderWidget);
	}

	bool Viewport::isRendererSpecified() const
	{
		// Find Renderer Node inside the content manager
		const core::NodeGraph& nodeGraph = m_contentManager->getMainNodeGraph();
		for (auto node : nodeGraph.getNodes())
		{
			if (std::shared_ptr<renderer::Renderer> renderer = std::dynamic_pointer_cast<renderer::Renderer>(node))
			{
				return true;
			}
		}

		return false;
	}

} // namespace st::ui