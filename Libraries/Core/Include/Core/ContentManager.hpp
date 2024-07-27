#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "Handlers.hpp"
#include "NodeGraph.hpp"

namespace st::core
{

	class ContentManager
	{
	  public:
		ContentManager();

		/*----------------------*/
		/*-------Methods--------*/
		/*----------------------*/
		bool initialize();
		bool onStart();

		/*----------------------*/
		/*--------Setters-------*/
		/*----------------------*/
		NodeGraphHandler getMainNodeGraph();


	  private:
		NodeGraph m_nodeGraph;
	};

	using ContentManagerHandler = Handler<ContentManager>;

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP