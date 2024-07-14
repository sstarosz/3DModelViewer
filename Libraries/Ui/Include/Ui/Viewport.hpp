#ifndef ST_UI_VIEWPORT_HPP
#define ST_UI_VIEWPORT_HPP

#include <QWidget>
#include <Core/ContentManager.hpp>

namespace st
{
namespace core
{
	class Scene;
}

namespace renderer
{
	class Renderer;
}

namespace ui
{

	class Viewport : public QWidget
	{
		Q_OBJECT

	  public:
		explicit Viewport(
			core::ContentManagerHandler contentManager,
			QWidget* parent = nullptr, 
		Qt::WindowFlags flags = {});

	  private:
		class PrivateWindow;
		PrivateWindow* m_window;
	};

} // namespace ui
} // namespace st

#endif // ST_UI_VIEWPORT_HPP
