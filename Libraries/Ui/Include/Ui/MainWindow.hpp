#ifndef ST_UI_MAINWINDOW_HPP
#define ST_UI_MAINWINDOW_HPP

#include <QMainWindow>

namespace st
{

namespace core
{
	class ContentManager;
}

namespace ui
{
	class Viewport;
	class NodeEditor;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	  public:
		explicit MainWindow(core::ContentManager& contentManager, QWidget* parent = nullptr, Qt::WindowFlags flags = {});

		void initialize();
	  private:
		core::ContentManager& m_contentManager;
		Viewport* m_viewport;
		NodeEditor* m_nodeEditor;
	};

} // namespace ui
} // namespace st

#endif // ST_UI_MAINWINDOW_HPP