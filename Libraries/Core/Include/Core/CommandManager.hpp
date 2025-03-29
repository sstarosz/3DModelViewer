#ifndef ST_CORE_COMMANDMANAGER_HPP
#define ST_CORE_COMMANDMANAGER_HPP

#include "Core/Command.hpp"
#include "Core/Handlers.hpp"
#include <spdlog/spdlog.h>
#include <vector>
#include <memory>

namespace st::core
{
	class AbstractCommand
	{
	public:
		virtual void undo() = 0;
	};

	template <typename ResultT>
	class CommandWrapper : public AbstractCommand {
	public:
		CommandWrapper(std::unique_ptr<Command<ResultT>> cmd) : 
			cmd(std::move(cmd)){}

		void undo() override {
			cmd->undo();
		}

	private:
		std::unique_ptr<Command<ResultT>> cmd;
	};

    
	class CommandManager
	{
		public:
		CommandManager();

		void initialize();

		template <typename CommandType>
		auto execute(std::unique_ptr<CommandType> command)
		{
			using ResultType = typename CommandType::ResultType;

			if constexpr (std::is_same_v<ResultType, void>)
			{
				command->execute();
				auto wrapper = std::make_unique<CommandWrapper<ResultType>>(std::move(command));
				commands.push_back(std::move(wrapper));
				undoneCommands.clear();
				return;
			}
			else
			{
				ResultType result = command->execute();
				auto wrapper = std::make_unique<CommandWrapper<ResultType>>(std::move(command));
				commands.push_back(std::move(wrapper));
				undoneCommands.clear();
				return result;
			}
		}

		void undo();

	  private:
	  std::vector<std::unique_ptr<AbstractCommand>> commands;
	  std::vector<std::unique_ptr<AbstractCommand>> undoneCommands;
	};

    using CommandManagerHandler = Handler<CommandManager>;

} // namespace st::core

#endif // ST_CORE_COMMANDMANAGER_HPP