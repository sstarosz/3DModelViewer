#include "Viewport.hpp"
#include <QHBoxLayout>
#include <QTimer>
#include <QVulkanInstance>
#include <QWindow>
#include <QLabel>

#include "Renderer/Renderer.hpp"

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
		QWidget* renderWidget{nullptr};

		if(isRendererSpecified())
		{
			m_window = new PrivateWindow(contentManager);
			renderWidget = QWidget::createWindowContainer(m_window, this);
		}
		else
		{
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