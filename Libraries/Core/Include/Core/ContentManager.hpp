#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "Handlers.hpp"
#include "NodeGraph.hpp"
#include "EventRegistry.hpp"

namespace st::core
{
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

		void updateSelection(std::weak_ptr<Node> node);
		std::weak_ptr<Node> getSelectedNode() const;

	  private:
		NodeGraph m_nodeGraph;
		std::weak_ptr<Node> m_selectedNode; //TODO implement selection system
	};

	using ContentManagerHandler = Handler<ContentManager>;

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP