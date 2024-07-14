#ifndef ST_UI_MAINWINDOW_HPP
#define ST_UI_MAINWINDOW_HPP

#include <QMainWindow>
#include "Core/ContentManager.hpp"

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
			explicit MainWindow(core::ContentManagerHandler contentManager,
								QWidget* parent = nullptr,
								Qt::WindowFlags flags = {});

			void initialize();

		  private:
			core::ContentManagerHandler m_contentManager;
			Viewport* m_viewport;
			NodeEditor* m_nodeEditor;
		};

	} // namespace ui
} // namespace st

#endif // ST_UI_MAINWINDOW_HPP