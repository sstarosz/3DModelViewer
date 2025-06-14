#include "Application.hpp"

#include <spdlog/spdlog.h>


namespace st::application
{
    Application::Application(int argc, char* argv[]) :
        m_state(ApplicationState::eUninitialized),
        m_app(argc, argv),
        m_commandManager(),
        m_contentManager(),
        m_guiManager(core::ContentManagerHandler(&m_contentManager)),
        m_creator(core::ContentManagerHandler(&m_contentManager), core::CommandManagerHandler(&m_commandManager))
    {
    }
    
    int Application::start()
    {
        spdlog::info("Application::start()");

        m_state = ApplicationState::eInitialized;

        if(m_sceneBuilder)
        {
            spdlog::info("Building scene...");
            m_sceneBuilder();
        }

        m_contentManager.onStart();

        m_guiManager.show();
        m_state = ApplicationState::eRunning;

        spdlog::info("Application::run() - Done");
        return m_app.exec();
    }

    Creator& Application::create()
    {
        return m_creator;
    }

    Modifier Application::modify(std::shared_ptr<core::Node> node)
    {
        ModifyContext context;
        context.m_contentManager = &m_contentManager;
        context.selectedNode = node;

        Modifier modifier{context, core::CommandManagerHandler(&m_commandManager)};
        return modifier;
    }

} // namespace st::application

