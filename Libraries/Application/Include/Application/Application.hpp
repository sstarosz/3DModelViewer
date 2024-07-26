#ifndef ST_APPLICATION_APPLICATION
#define ST_APPLICATION_APPLICATION

#include <Eigen/Core>
#include <spdlog/spdlog.h>
#include <vector>

namespace st::application
{
    
	class Application
	{
	  public:
		Application(int argc, char* argv[]) :
			m_app(argc, argv),
			m_commandManager(),
			m_contentManager(),
			m_guiManager(core::ContentManagerHandler(&m_contentManager)),
			m_creator(core::ContentManagerHandler(&m_contentManager), CommandManagerHandler(&m_commandManager))
		{
			spdlog::set_level(spdlog::level::debug);
			spdlog::info("Application::Application()");
			spdlog::info("Application::Application() - Done");
			spdlog::info("----------------------");
		}


		int initialize()
		{
			spdlog::info("Application::initialize()");
			m_contentManager.initialize();
			m_commandManager.initialize();
			m_guiManager.initialize();

			spdlog::info("Application::initialize() - Done");
			spdlog::info("----------------------");
			return 0;
		}


		int run()
		{	
			spdlog::info("Application::run()");	

			m_contentManager.onStart();
			
			m_guiManager.show();

			return m_app.exec();
		}

		Creator& create()
		{
			return m_creator;
		}

		Modifier modify(std::weak_ptr<core::Node> node)
		{
			ModifyContext context;
            context.m_contentManager = &m_contentManager;
			context.selectedNode = node;


			Modifier modifier{context, CommandManagerHandler(&m_commandManager)};
			return modifier;
		}


	  private:
		QApplication m_app;

		//Communication withing the application
		CommandManager m_commandManager;


		// Content
		core::ContentManager m_contentManager;
		Creator m_creator;

		// Gui
		GuiManager m_guiManager;

		// GuiCreator m_guiCreator; To investigate future use
	};

} // namespace st::application


#endif // ST_APPLICATION_APPLICATION
