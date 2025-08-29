#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "Handlers.hpp"
#include "NodeGraph.hpp"
#include "EventRegistry.hpp"

namespace st::core
{
	class Selection
	{
		public:
		std::vector<Path> m_paths;
	};
	
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
		bool onStart();

		/*----------------------*/
		/*--------Setters-------*/
		/*----------------------*/
		NodeGraph& getMainNodeGraph();
		const NodeGraph& getMainNodeGraph() const;

		void updateSelection(std::weak_ptr<Node> node);
		Selection getSelection() const;

	  private:
		NodeGraph m_nodeGraph;
		Selection m_selection;
	};

	using ContentManagerHandler = Handler<ContentManager>;

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP