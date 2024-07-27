#ifndef ST_UI_GUIMANAGER_HPP
#define ST_UI_GUIMANAGER_HPP

#include "Core/ContentManager.hpp"

#include "Ui/MainWindow.hpp"

namespace st::ui
{
class GuiManager
	{
	  public:
		GuiManager(core::ContentManagerHandler contentManager);

	  public:
		void initialize();
		void show();

	  private:
		core::ContentManagerHandler m_contentManager;
		ui::MainWindow m_mainWindow;
	};

} // namespace st::ui

#endif // ST_UI_GUIMANAGER_HPP