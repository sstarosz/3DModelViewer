#include "NodeEditor.hpp"
#include "Core/ContentManager.hpp"
#include "Core/Nodes/Node.hpp"
#include "Core/NodeGraph.hpp"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <print>
#include <QScrollArea>
#include <QScrollBar>

namespace st::ui
{

/*------------------------------------*/
/*-----------NodeScene-----------------*/
/*------------------------------------*/
NodeScene::NodeScene(QObject* parent) : QGraphicsScene(parent)
{
}


void NodeScene::drawBackground(QPainter* painter, const QRectF& rect) 
{
    Q_UNUSED(rect);

    
    constexpr int32_t sceneWidth = 10000;
    constexpr int32_t sceneHeight = 10000;

    //Draw grid lines
    painter->setPen(Qt::lightGray);
    for(int32_t x = -sceneWidth;  x <= sceneWidth; x += 10)
    {
        painter->drawLine(x * 10, -sceneHeight, x * 10, sceneHeight);
    }

    for(int32_t y = -sceneHeight;  y <= sceneHeight; y += 10)
    {
        painter->drawLine(-sceneWidth, y * 10, sceneWidth, y * 10);
    }


    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    addLine(-sceneWidth, 0, sceneWidth, 0, axisPen);
    addLine(0, -sceneHeight, 0, sceneHeight, axisPen);

}

/*------------------------------------*/
/*-----------NodeEditor---------------*/
/*------------------------------------*/


NodeEditor::NodeEditor(core::ContentManager& contentManager, QWidget* parent):
    QGraphicsView(parent),
    m_scene(new NodeScene(this)),
    m_contentManager(contentManager),
    m_panning(false),
    m_lastPanPoint(QPoint(0, 0))
    {

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setScene(m_scene);
    }

    void NodeEditor::initialize()
    {
        m_scene->setView(this);
        setupScene();
        centerOn(0,0);

    }

	void NodeEditor::setupScene()
    {
        auto nodes = m_contentManager.getNodes();
        std::println("!!!NodeGraph size: {}", nodes.size());

        //Display one node for now
        auto node = nodes.at(0);

        //std::println("!!!!Node: {}", core::Node::toString(node->m_type));

        NodeItem* nodeItem = new NodeItem(node);
        nodeItem->setPosition(QPointF(100, -100));

        m_scene->addItem(nodeItem);

    }

	void NodeEditor::mousePressEvent(QMouseEvent* event)
    {
       if (event->button() == Qt::MiddleButton) 
        {
            m_panning = true;
            m_lastPanPoint = event->position();
            setCursor(Qt::ClosedHandCursor);
        } 

        QGraphicsView::mousePressEvent(event);
    }

    void NodeEditor::mouseMoveEvent(QMouseEvent* event)
    {
         if (m_panning) 
         {
            QPointF delta = event->position() - m_lastPanPoint;
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        
            m_lastPanPoint = event->position();
        } 


        QGraphicsView::mouseMoveEvent(event);
    }

    void NodeEditor::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::MiddleButton) 
        {
            m_panning = false;
            setCursor(Qt::ArrowCursor); 
        } 

        QGraphicsView::mouseReleaseEvent(event);
    }

	void NodeEditor::wheelEvent(QWheelEvent* event)
	{
        double angleDeltaY = event->angleDelta().y();

        double factor = std::pow(1.0015, angleDeltaY);

        scale(factor, factor);

        QGraphicsView::wheelEvent(event);
	}

	void NodeEditor::resizeEvent(QResizeEvent* event)
	{
        QGraphicsView::resizeEvent(event);
	}

} // namespace st::ui
