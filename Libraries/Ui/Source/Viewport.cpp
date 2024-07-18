#include "Viewport.hpp"
#include <QDebug>
#include <QHBoxLayout>
#include <QTimer>
#include <QVulkanInstance>
#include <QWindow>
#include <QOpenGLWidget>

#include "Renderer/Renderer.hpp"
#include "Core/Scene.hpp"

namespace st::ui
{
	/*---------------------*/
	/*-------Private-------*/
	/*---------------------*/
	class Viewport::PrivateWindow : public QWindow
	{
	  public:
		PrivateWindow(
			core::ContentManagerHandler contentManager,
			QWindow* parent = nullptr) :
			QWindow(parent),
			m_contentManager(contentManager)
		{
			m_contentManager = contentManager;
			setSurfaceType(QSurface::VulkanSurface);
			setMinimumSize(QSize(1,1));
		}

		void showEvent(QShowEvent* event) override
		{
			QWindow::showEvent(event);

			if (!m_initialized)
			{
				//Find Renderer Node inside the content manager
				core::NodeGraphHandler nodeGraph = m_contentManager->getMainNodeGraph();
				for(auto node : nodeGraph.getNodes())
				{
					if (std::shared_ptr<renderer::Renderer> renderable = std::dynamic_pointer_cast<renderer::Renderer>(node))
					{
						spdlog::info("Renderer Node found");
						m_renderer2 = renderable;
					}
				}


				//Create Vulkan Instance for QVulkanWindow
				QVulkanInstance vulkanInstance;
				vulkanInstance.setVkInstance(m_renderer2->getVulkanInstance());
				if (!vulkanInstance.create())
				{
					qDebug() << "Failed to create vulkan instance";
					return;
				}
				setVulkanInstance(&vulkanInstance);

				//Embed Surface to Renderer
				m_renderer2->setSurface(vulkanInstance.surfaceForWindow(this));
				//m_renderer2->init();

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
			m_renderer2->changeSwapchainExtent(width(), height());
		}

		void setRenderer(std::shared_ptr<renderer::Renderer> renderer)
		{
			m_renderer2 = renderer;
		}

		void setScene(core::Scene&& scene)
		{
			m_scene = std::move(scene);
		}

	  private:
		// Main loop
		void update()
		{
			if (m_initialized)
			{
				m_renderer2->render(m_scene);


				requestUpdate();
			}
		}

		core::ContentManagerHandler m_contentManager;
		bool m_initialized{false};
		QTimer m_timer;
		//renderer::Renderer m_renderer;
		std::shared_ptr<renderer::Renderer> m_renderer2;
		core::Scene m_scene;
	};

	/*---------------------*/
	/*-------Public--------*/
	/*---------------------*/
	Viewport::Viewport(
		core::ContentManagerHandler contentManager,
		QWidget* parent, 
		Qt::WindowFlags flags) :
		QWidget(parent, flags),
		m_window(new PrivateWindow(contentManager))
	{
		QWidget* vulkanWidget = QWidget::createWindowContainer(m_window, this);
		
		QHBoxLayout* layout = new QHBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(vulkanWidget);
	}

} // namespace st::ui