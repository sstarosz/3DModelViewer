#include "NodeEditor.hpp"
#include "Core/ContentManager.hpp"
#include "Core/Nodes/Node.hpp"
#include "Core/NodeGraph.hpp"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <print>

namespace st::ui
{

NodeScene::NodeScene(QObject* parent) : QGraphicsScene(parent)
{
}


void NodeScene::drawBackground(QPainter* painter, const QRectF& rect) 
{
    Q_UNUSED(rect);

    painter->setTransform(m_view->viewportTransform());

    //Draw grid lines
    painter->setPen(Qt::lightGray);
    for(int x = -100;  x <= 100; x += 10)
    {
        painter->drawLine(x * 10, -1000, x * 10, 1000);
    }

    for(int y = -100;  y <= 100; y += 10)
    {
        painter->drawLine(-1000, y * 10, 1000, y * 10);
    }

    //Draw axes
    painter->setPen(Qt::black);
    painter->drawLine(0, -1000, 0, 1000);
    painter->drawLine(-1000, 0, 1000, 0);

}


NodeEditor::NodeEditor(core::ContentManager& contentManager, QWidget* parent):
    QGraphicsView(parent),
    m_scene(new NodeScene(this)),
    m_contentManager(contentManager),
    m_lastPanPoint(QPoint(0, 0)),
    m_yAxis(nullptr),
    m_xAxis(nullptr)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        setStyleSheet("border: none;");
        setContentsMargins(0, 0, 0, 0);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setScene(m_scene);
    }

    void NodeEditor::initialize()
    {
        m_scene->setView(this);
        setupScene();
        m_xAxis = m_scene->addLine(0, 0, 100, 0);
        m_yAxis = m_scene->addLine(0, 0, 0, 100);
        drawAxes();
    }

	void NodeEditor::drawAxes()
	{
		if (!m_scene)
		{
			std::println("Scene is null");
			return;
		}

		// Get the viewport's rect in scene coordinates
		QRectF viewportRect = mapToScene(viewport()->rect()).boundingRect();

		// Style
		QPen axisPen(Qt::black);
		axisPen.setWidth(2);

		// Remove existing axes if they exist
		if (m_xAxis)
		{
			m_scene->removeItem(m_xAxis);
		}

		if (m_yAxis)
		{
			m_scene->removeItem(m_yAxis);
		}

		// Draw the x-axis
		m_xAxis = m_scene->addLine(viewportRect.left(), 0, viewportRect.right(), 0, axisPen);

		// Draw the y-axis
		m_yAxis = m_scene->addLine(0, viewportRect.top(), 0, viewportRect.bottom(), axisPen);
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
            m_lastPanPoint = event->pos();
            setCursor(Qt::ClosedHandCursor);
        } 
        else 
        {
            QGraphicsView::mousePressEvent(event);
        }
    }

    void NodeEditor::mouseMoveEvent(QMouseEvent* event)
    {
         if (event->buttons() & Qt::MiddleButton) 
         {
            QPointF delta = mapToScene(m_lastPanPoint) - mapToScene(event->pos());
            translate(delta.x(), delta.y());
            m_lastPanPoint = event->pos();
        } 
        else 
        {
            QGraphicsView::mouseMoveEvent(event);
        }

        QGraphicsView::mouseMoveEvent(event);
    }

    void NodeEditor::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::MiddleButton) 
        {
            setCursor(Qt::ArrowCursor);
        } 
        else 
        {
            QGraphicsView::mouseReleaseEvent(event);
        }

        QGraphicsView::mouseReleaseEvent(event);
    }

	void NodeEditor::resizeEvent(QResizeEvent* event)
	{
        QGraphicsView::resizeEvent(event);
        drawAxes();
	}

} // namespace st::ui
