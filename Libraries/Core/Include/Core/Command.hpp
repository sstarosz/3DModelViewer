#ifndef ST_CORE_COMMAND_HPP
#define ST_CORE_COMMAND_HPP

namespace st::core
{
	template <typename ResultT>
	class Command
	{
	  public:
		using ResultType = ResultT;

		virtual ~Command() = default;

		virtual ResultT execute() = 0;
		virtual void undo() = 0;
	};
} // namespace st::core

#endif // ST_CORE_COMMAND_HPP