#ifndef ST_UI_MAINWINDOW_HPP
#define ST_UI_MAINWINDOW_HPP

#include "Core/ContentManager.hpp"
#include <QMainWindow>

namespace st::ui
{
	class Viewport;
	class NodeEditor;
	class AttributeEditor;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	  public:
		explicit MainWindow(core::ContentManagerHandler contentManager,
							QWidget* parent = nullptr,
							Qt::WindowFlags flags = {});

		void initialize();

	  private:
		core::ContentManagerHandler m_contentManager;
		Viewport* m_viewport;
		NodeEditor* m_nodeEditor;
		AttributeEditor* m_attributeEditor;
	};

} // namespace st::ui

#endif // ST_UI_MAINWINDOW_HPP