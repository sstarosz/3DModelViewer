#ifndef ST_APPLICATION_APPLICATION
#define ST_APPLICATION_APPLICATION

#include <Eigen/Core>
#include <spdlog/spdlog.h>
#include <vector>

#include <QApplication>
#include "Core/CommandManager.hpp"
#include "Core/ContentManager.hpp"
#include "Application/Creator.hpp"
#include "Application/Modifier.hpp"

#include "Ui/GuiManager.hpp"

namespace st::application
{

	class Application
	{
	  public:
		Application(int argc, char* argv[]);


		int initialize();

		int run();

		Creator& create();

		Modifier modify(std::weak_ptr<core::Node> node);


	  private:
		QApplication m_app;

		//Communication withing the application
		core::CommandManager m_commandManager;
		core::ContentManager m_contentManager;

		// Content
		Creator m_creator;

		// Gui
		ui::GuiManager m_guiManager;

		// GuiCreator m_guiCreator; To investigate future use
	};

} // namespace st::application


#endif // ST_APPLICATION_APPLICATION
