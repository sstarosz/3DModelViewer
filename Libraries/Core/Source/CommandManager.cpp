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

    //void CommandManager::execute(std::unique_ptr<Command> command)
    //{
    //    command->execute();
    //    m_undoStack.push_back(std::move(command));
    //    m_redoStack.clear();
    //}

    void CommandManager::undo()
    {
        if (!commands.empty()) {
            commands.back()->undo();
            undoneCommands.push_back(std::move(commands.back()));
            commands.pop_back();
        }
    }
 
}