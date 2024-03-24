#ifndef ST_UI_VIEWPORT_HPP
#define ST_UI_VIEWPORT_HPP

#include <QWidget>

namespace st::core
{
	class Scene;
}

namespace st::renderer
{
	class Renderer;
}

namespace st::ui
{

	class Viewport : public QWidget
	{
		Q_OBJECT

	  public:
		explicit Viewport(QWidget* parent = nullptr, Qt::WindowFlags flags = {});

		void embedRenderer(renderer::Renderer&& renderer);
		void embedScene(core::Scene&& scene);

	  private:
		class PrivateWindow;
		PrivateWindow* m_window;
	};

} // namespace st::ui

#endif // ST_UI_VIEWPORT_HPP
