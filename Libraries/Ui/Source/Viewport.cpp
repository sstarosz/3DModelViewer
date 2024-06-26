#include "Viewport.hpp"
#include <QDebug>
#include <QHBoxLayout>
#include <QTimer>
#include <QVulkanInstance>
#include <QWindow>

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
		PrivateWindow()
		{
			setSurfaceType(QSurface::VulkanSurface);
		}

		void showEvent(QShowEvent* event) override
		{
			QWindow::showEvent(event);

			if (!m_initialized)
			{
				vk::Instance instance = m_renderer.createInstance();

				QVulkanInstance vulkanInstance;
				vulkanInstance.setVkInstance(instance);

				if (!vulkanInstance.create())
				{
					qDebug() << "Failed to create vulkan instance";
					return;
				}
				setVulkanInstance(&vulkanInstance);

				m_renderer.setSurface(vulkanInstance.surfaceForWindow(this));
				m_renderer.init();

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
			m_renderer.changeSwapchainExtent(width(), height());
		}

		void setRenderer(renderer::Renderer&& renderer)
		{
			m_renderer = std::move(renderer);
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
				m_renderer.render(m_scene);


				requestUpdate();
			}
		}

		bool m_initialized = false;
		QTimer m_timer;
		renderer::Renderer m_renderer;
		core::Scene m_scene;
	};

	/*---------------------*/
	/*-------Public--------*/
	/*---------------------*/
	Viewport::Viewport(QWidget* parent, Qt::WindowFlags flags) :
		QWidget(parent, flags),
		m_window(new PrivateWindow)
	{
		QWidget* vulkanWidget = QWidget::createWindowContainer(m_window, this);

		QHBoxLayout* layout = new QHBoxLayout(this);
		layout->addWidget(vulkanWidget);
	}

	void Viewport::embedRenderer(renderer::Renderer&& renderer)
	{
		m_window->setRenderer(std::move(renderer));
	}

	void Viewport::embedScene(core::Scene&& scene)
	{
		m_window->setScene(std::move(scene));
	}

} // namespace st::ui