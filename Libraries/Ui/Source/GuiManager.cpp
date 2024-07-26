#include "GuiManager.hpp"

namespace st::ui
{
    GuiManager::GuiManager(core::ContentManagerHandler contentManager) :
        m_contentManager(contentManager),
        m_mainWindow(contentManager)
    {
        spdlog::info("GuiManager::GuiManager()");
    }

    void GuiManager::initialize()
    {
        spdlog::info("GuiManager::initialize()");
        m_mainWindow.initialize();
    }

    void GuiManager::show()
    {
        m_mainWindow.show();
    }

} // namespace st::ui
