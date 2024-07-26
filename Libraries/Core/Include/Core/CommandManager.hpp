#ifndef ST_CORE_COMMANDMANAGER_HPP
#define ST_CORE_COMMANDMANAGER_HPP

#include "Core/Command.hpp"
#include "Core/Handlers.hpp"
#include <spdlog/spdlog.h>
#include <vector>
#include <memory>

namespace st::core
{
    
	class CommandManager
	{
		public:
		CommandManager();

		void initialize();
		void execute(std::unique_ptr<Command> command);
		void undo();
		void redo();



	  private:
		std::vector<std::unique_ptr<Command>> m_undoStack;
		std::vector<std::unique_ptr<Command>> m_redoStack;
	};

    using CommandManagerHandler = Handler<CommandManager>;

} // namespace st::core

#endif // ST_CORE_COMMANDMANAGER_HPP