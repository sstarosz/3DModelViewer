#ifndef ST_CORE_COMMAND_HPP
#define ST_CORE_COMMAND_HPP

namespace st::core
{
	class Command
	{
	  public:
		Command() = default;
		virtual ~Command() = default;

		virtual void execute() = 0;
		virtual void undo() = 0;
	};
} // namespace st::core

#endif // ST_CORE_COMMAND_HPP