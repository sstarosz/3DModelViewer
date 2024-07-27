#ifndef ST_APPLICATION_MODIFIER_HPP
#define ST_APPLICATION_MODIFIER_HPP

#include "Core/Node.hpp"
#include "Core/CommandManager.hpp"
#include "Core/ContentManager.hpp"
#include "Renderer/Nodes/StandardMaterial.hpp"

namespace st::application
{

	class ModifyContext
	{
	  public:
		core::ContentManager* m_contentManager;
		std::weak_ptr<core::Node> selectedNode;
	};

	class MaterialModifier
	{
	  public:
		MaterialModifier(ModifyContext context, core::CommandManagerHandler commandManager);
		void assign(std::weak_ptr<renderer::StandardMaterial> material);

	  private:
		ModifyContext m_context;
		core::CommandManagerHandler m_commandManager;
	};

	class Modifier
	{
	  public:
		Modifier(ModifyContext context,
				 core::CommandManagerHandler commandManager);

		// TODO refactor
		void transform();

		MaterialModifier material();

	  private:
		ModifyContext m_context;
		core::CommandManagerHandler m_commandManager;
	};
} // namespace st::application

#endif // ST_APPLICATION_MODIFIER_HPP