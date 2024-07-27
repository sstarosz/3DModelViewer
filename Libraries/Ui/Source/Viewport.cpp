#include "Viewport.hpp"
#include <QHBoxLayout>
#include <QTimer>
#include <QVulkanInstance>
#include <QWindow>

#include "Renderer/Renderer.hpp"

namespace st::ui
{
	/*---------------------*/
	/*-------Private-------*/
	/*---------------------*/
	class Viewport::PrivateWindow : public QWindow
	{
	  public:
		PrivateWindow(core::ContentManagerHandler contentManager,
					  QWindow* parent = nullptr) :
			QWindow(parent),
			m_contentManager(contentManager)
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
				core::NodeGraphHandler nodeGraph = m_contentManager->getMainNodeGraph();
				for (auto node : nodeGraph->getNodes())
				{
					if (std::shared_ptr<renderer::Renderer> renderable = std::dynamic_pointer_cast<renderer::Renderer>(node))
					{
						spdlog::info("Renderer Node found");
						m_renderer = renderable;
					}
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