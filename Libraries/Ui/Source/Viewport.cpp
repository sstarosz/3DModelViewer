#include "Viewport.hpp"
#include <QHBoxLayout>
#include <QTimer>
#include <QVulkanInstance>
#include <QWindow>
#include <QLabel>
#include <QMouseEvent>

#include "Renderer/Renderer.hpp"

#include "Core/Nodes/CameraNode.hpp"
#include "Core/Nodes/TransformNode.hpp"

#include <numbers>

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
		enum class CameraState
		{
			eIdle,
			eOrbit,
			ePan,
			eZoom
		};


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
				

				spdlog::info("Window size: y:{}, x:{}", width(), height());
				if(auto parentNode = m_camera->getParentNode().lock())
				{
					spdlog::info("Camera Parent Node found");
					m_transform = std::dynamic_pointer_cast<core::TransformNode>(parentNode);
				}
				else
				{
					assert(false && "Camera Parent Node not found");
				}


				m_camera->setWidth(static_cast<float>(width()));
				m_camera->setHeight(static_cast<float>(height()));

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
			m_camera->setWidth(static_cast<float>(width()));
			m_camera->setHeight(static_cast<float>(height()));
			m_camera->compute();
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

			if(event->modifiers() & Qt::AltModifier)
			{
				spdlog::info("Alt Modifier Pressed");

				m_lastMousePosition = event->position();
				if(event->button() == Qt::LeftButton)
				{
					spdlog::info("Alt + Left Button Pressed");
					m_cameraState = CameraState::eOrbit;
				}

				if(event->button() == Qt::MiddleButton)
				{
					spdlog::info("Alt + Middle Button Pressed");
					m_cameraState = CameraState::ePan;
				}

				if(event->button() == Qt::RightButton)
				{
					spdlog::info("Alt + Right Button Pressed");
					m_cameraState = CameraState::eZoom;
				}
			}
		}

		void mouseMoveEvent(QMouseEvent* event) override
		{
			if(m_cameraState == CameraState::eIdle)
			{
				return;
			}

			auto pos = event->position();
			float deltaX = pos.x() - m_lastMousePosition.x();
			float deltaY = pos.y() - m_lastMousePosition.y();


			if(m_cameraState == CameraState::eOrbit)
			{
				orbit(deltaX, deltaY);
			}

			if(m_cameraState == CameraState::ePan)
			{
				pan(deltaX, deltaY);
			}

			if(m_cameraState == CameraState::eZoom)
			{
				dolly(deltaX, deltaY);
			}

			m_lastMousePosition = pos;


			// Evaluate the node graph
			m_transform->compute();
			m_camera->compute();
		}

		void mouseReleaseEvent([[maybe_unused]] QMouseEvent* event) override
		{
			update();
			m_cameraState = CameraState::eIdle;
		}

		float degreeToRadian(float degree) const
		{
			return degree * (std::numbers::pi_v<float> / 180.0f);
		}

		void orbit(float deltaX, float deltaY)
		{
			float width = static_cast<float>(this->width());
			float height = static_cast<float>(this->height());

			float angleX = -deltaX / (width / 2.0) * 360.0f;
			float angleY = -deltaY / (height / 2.0) * 360.0f;

			spdlog::warn("Orbit: angleX: {}, angleY: {}", angleX, angleY);

			m_transform->rotateX(angleY);
			m_transform->rotateY(angleX);
		}

		void pan(float deltaX, float deltaY)
		{
			constexpr float sensitivity = 0.01f;
			spdlog::warn("Panning: deltaX: {}, deltaY: {}", deltaX, deltaY);	
			m_transform->translateBy(Eigen::Vector4f{-deltaX * sensitivity, -deltaY * sensitivity, 0.0f, 0.0f});
		}

		void dolly(float deltaX, float deltaY)
		{
			constexpr float sensitivity = 0.01f;
			spdlog::warn("Zooming: deltaX: {}, deltaY: {}", deltaX, deltaY);
			m_transform->translateBy(Eigen::Vector4f{0.0f, 0.0f, -deltaY * sensitivity, 0.0f});
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
		std::shared_ptr<core::TransformNode> m_transform; //Transform node of the camera
		QLabel* m_fallbackLabel{nullptr};
		CameraState m_cameraState{CameraState::eIdle};

		QPointF m_lastMousePosition;
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