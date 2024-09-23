#include "Application.hpp"

#include <spdlog/spdlog.h>


namespace st::application
{
    Application::Application(int argc, char* argv[]) :
        m_app(argc, argv),
        m_commandManager(),
        m_contentManager(),
        m_guiManager(core::ContentManagerHandler(&m_contentManager)),
        m_creator(core::ContentManagerHandler(&m_contentManager), core::CommandManagerHandler(&m_commandManager))
    {
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("Application::Application()");
        spdlog::info("Application::Application() - Done");
        spdlog::info("----------------------");
    }

    int Application::initialize()
    {
        spdlog::info("Application::initialize()");
        m_contentManager.initialize();
        m_commandManager.initialize();
        m_guiManager.initialize();

        spdlog::info("Application::initialize() - Done");
        spdlog::info("----------------------");
        return 0;
    }

    int Application::run()
    {
        spdlog::info("Application::run()");
        initialize();


        m_contentManager.onStart();

        m_guiManager.show();

        return m_app.exec();
    }

    Creator& Application::create()
    {
        return m_creator;
    }

    Modifier Application::modify(std::weak_ptr<core::Node> node)
    {
        ModifyContext context;
        context.m_contentManager = &m_contentManager;
        context.selectedNode = node;


        Modifier modifier{context, core::CommandManagerHandler(&m_commandManager)};
        return modifier;
    }

} // namespace st::application

