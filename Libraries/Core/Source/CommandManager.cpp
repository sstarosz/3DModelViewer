#include "CommandManager.hpp"

namespace st::core
{
    CommandManager::CommandManager()
    {
        spdlog::info("CommandManager::CommandManager()");
    }

    void CommandManager::initialize()
    {
        spdlog::info("CommandManager::initialize()");
    }

    void CommandManager::execute(std::unique_ptr<Command> command)
    {
        command->execute();
        m_undoStack.push_back(std::move(command));
        m_redoStack.clear();
    }

    void CommandManager::undo()
    {
        if(!m_undoStack.empty())
        {
            std::unique_ptr<Command> command = std::move(m_undoStack.back());
            m_undoStack.pop_back();
            command->undo();
            m_redoStack.push_back(std::move(command));
        }
    }

    void CommandManager::redo()
    {
        if(m_redoStack.empty())
        {
            std::unique_ptr<Command> command = std::move(m_redoStack.back());
            m_redoStack.pop_back();
            command->execute();
            m_undoStack.push_back(std::move(command));
        }
    }
    
}