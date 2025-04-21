#ifndef ST_CORE_CONTENTMANAGER_HPP
#define ST_CORE_CONTENTMANAGER_HPP

#include "Handlers.hpp"
#include "NodeGraph.hpp"
#include "EventRegistry.hpp"
#include <ranges>

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
		bool initialize();
		bool onStart();

		/*----------------------*/
		/*--------Setters-------*/
		/*----------------------*/
		NodeGraph& getMainNodeGraph();
		const NodeGraph& getMainNodeGraph() const;

		void updateSelection(std::weak_ptr<Node> node);
		std::weak_ptr<Node> getSelectedNode() const;

		void setSelectedNode(std::weak_ptr<Node> node)
		{
			m_selectedNode = node;
		}

		Selection getSelection() const
		{
			return m_selection;
		}


	  private:
		NodeGraph m_nodeGraph;
		Selection m_selection;
		std::weak_ptr<Node> m_selectedNode; //TODO implement selection system

	};

	using ContentManagerHandler = Handler<ContentManager>;

} // namespace st::core

#endif // ST_CORE_CONTENTMANAGER_HPP