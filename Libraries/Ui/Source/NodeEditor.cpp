#include "NodeEditor.hpp"
#include "Core/ContentManager.hpp"
#include "Core/NodeGraph.hpp"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <print>

namespace st::ui
{

NodeEditor::NodeEditor(core::ContentManager& contentManager, QWidget* parent):
    QGraphicsView(parent),
    m_scene(new QGraphicsScene(this)),
    m_contentManager(contentManager)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        setStyleSheet("border: none;");
        setContentsMargins(0, 0, 0, 0);

        setScene(m_scene);
    }

    void NodeEditor::initialize()
    {
        setupScene();
    }

    void NodeEditor::mousePressEvent(QMouseEvent* event)
    {
        QGraphicsView::mousePressEvent(event);
    }

    void NodeEditor::mouseMoveEvent(QMouseEvent* event)
    {
        QGraphicsView::mouseMoveEvent(event);
    }

    void NodeEditor::mouseReleaseEvent(QMouseEvent* event)
    {
        QGraphicsView::mouseReleaseEvent(event);
    }


    void NodeEditor::setupScene()
    {
        auto nodes = m_contentManager.getNodes();
        std::println("!!!NodeGraph size: {}", nodes.size());

        //Display one node for now
        auto node = nodes.at(0);

        std::println("!!!!Node: {}", core::Node::toString(node->m_type));

        NodeItem* nodeItem = new NodeItem(node);
        m_scene->addItem(nodeItem);

    }

}  // namespace st::ui

