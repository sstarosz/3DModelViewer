#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "Handlers.hpp"
#include "NodeGraph.hpp"
#include "EventRegistry.hpp"

namespace st::core
{
	/*----------------------*/
	/*-------Events--------*/
	/*----------------------*/
	constexpr EventId SelectionChanged = EventRegistry::instance().registerEvent("SelectionChanged");



	/*-----------------------------*/
	/*-------ContentManager--------*/
	/*-----------------------------*/
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
		NodeGraph& getMainNodeGraph();
		const NodeGraph& getMainNodeGraph() const;


	  private:
		NodeGraph m_nodeGraph;
		EventRegistry m_eventRegistry;
	};

	using ContentManagerHandler = Handler<ContentManager>;

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP