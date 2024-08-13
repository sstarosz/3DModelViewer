#include "NodeEditor.hpp"
#include "Core/ContentManager.hpp"
#include "Core/Node.hpp"
#include "Core/NodeGraph.hpp"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <print>

#include <spdlog/spdlog.h>

namespace st::ui
{
    /*-------------------------------------------*/
    /*-----------MARK: NodeNameBase--------------*/
    /*-------------------------------------------*/
	NodeNameBase::NodeNameBase(const QString& name, QGraphicsItem* parent) :
		QAbstractGraphicsShapeItem(parent),
		m_name(name)
	{
	}

	QRectF NodeNameBase::boundingRect() const
	{
		return QRectF(0, 0, 320, 45);
	}

	void NodeNameBase::paint(QPainter* painter,
							 const QStyleOptionGraphicsItem* option,
							 QWidget* widget)
	{
		// Draw black
		painter->setPen(QPen(NodeBorderColor, 3));
		painter->setBrush(NodeBorderColor);
		painter->drawRoundedRect(0, 0, 300, 45, 20, 20);
		painter->drawRect(0, 20, 300, 25);

		// Draw text
		painter->setPen(Qt::white);
		painter->setFont(QFont("Inter", 24));

		QRectF textRect(20, 10, 280, 35);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_name);
	}

	/*-------------------------------------------*/
    /*-----------MARK: NodePlug------------------*/
    /*-------------------------------------------*/
    NodePlug::NodePlug(QGraphicsItem* parent) : QAbstractGraphicsShapeItem(parent)
    {
        setAcceptHoverEvents(true);

        setBrush(nodeConnectionColor);
        setPen(Qt::NoPen);
    }

	QRectF NodePlug::boundingRect() const
	{
		return QRectF(0.0F, 0.0F, 25.0F, 25.0F);
	}

	QPainterPath NodePlug::createConnectionPath()
	{
		QPainterPath path;
		// Start at the first point
		path.moveTo(0, 12);

		// Smoth curve
		path.cubicTo(QPointF(0.0F, 9.0F), QPointF(3.0F, 2.0F), QPointF(15, 0));

		if (isConnected() || isUnderMouse() || m_isConnecting)
		{
			path.lineTo(21, 0);
			path.lineTo(21, 25);
		}

		// Line down
		path.lineTo(15, 25);

		// Smoth curve
		path.cubicTo(QPointF(3.0F, 22.0F), QPointF(0.0F, 17.0F), QPointF(0, 14));

		// Line close
		path.lineTo(0, 12);

		path.closeSubpath();
		return path;
	}

	void NodePlug::paint(QPainter* painter,
						 const QStyleOptionGraphicsItem* option,
						 QWidget* widget)
	{
		Q_UNUSED(option);
		Q_UNUSED(widget);

		painter->setPen(pen());
		painter->setBrush(brush());

		QPainterPath path = createConnectionPath();
		painter->drawPath(path);
	}

	void NodePlug::setConnected(bool state)
	{
		m_isConnected = state;
	}

	bool NodePlug::isConnected() const
	{
		return m_isConnected;
	}

	void NodePlug::beginConnection()
	{
		m_isConnecting = true;
	}

	void NodePlug::endConnection()
	{
		m_isConnecting = false;
	}

	NodeAttribute* NodePlug::getParentAttribute() const
    {
        return dynamic_cast<NodeAttribute*>(parentItem());
    }

	QPointF NodePlug::getPlugCenterPosition() const
	{
		// TODO refactor
		return scenePos() + QPointF(0, 12.5);
	}

	/*-------------------------------------------*/
    /*-----------MARK: NodeAttribute-------------*/
    /*-------------------------------------------*/
	NodeAttribute::NodeAttribute(std::shared_ptr<core::Attribute> attribute, QGraphicsItem* parent) :
		QAbstractGraphicsShapeItem(parent),
		m_attribute(attribute),
		m_pInputPlug(nullptr),
		m_pOutputPlug(nullptr)
	{
		setAcceptHoverEvents(true);

		setBrush(nodeLabelColor);
		setPen(Qt::NoPen);

		// Draw connection Input
		if (m_attribute->isWritable())
		{
			NodePlug* nodePlug = new NodePlug(this);
			nodePlug->setPos(0, 0);
			m_pInputPlug = nodePlug;
		}

		// Draw Connection Output
		if (m_attribute->isReadable())
		{
            //TODO validate transform
			NodePlug* nodePlug = new NodePlug(this);
			QTransform transform;
			transform.translate(320, 0);
			transform.scale(-1, 1);
			nodePlug->setTransform(transform);
			m_pOutputPlug = nodePlug;
		}
	}

    QRectF NodeAttribute::boundingRect() const
    {
        return QRectF(0, 0, nodeWidth, nodeHeight);
    }

	void NodeAttribute::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		// TODO color of connection that depend on type of the handler
		// TODO connection and disconnection animation

		// Draw Label
		painter->setPen(pen());
		painter->setBrush(brush());
		painter->drawRect(20,
						  0,
						  nodeLabelWidth,
						  nodeLabelHeight);

		// Draw connection end
		painter->setBrush(nodeConnectionColor);

		// Draw connection input
		painter->drawRect(20, 0, 5, 25);

		// Draw connection output
		painter->drawRect(295, 0, 5, 25);

		// Draw text
		painter->setPen(Qt::white);
		painter->setFont(QFont("Inter", 12));
		QRectF textRect{45, 5, 230, 15};
		painter->drawText(textRect,
						  Qt::AlignLeft | Qt::AlignVCenter,
						  QString::fromStdString(m_attribute->getName()));

		// If int or float
		// Create input bar

		// If bool
		// Create switch
	}

    void NodeAttribute::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
    {
        update();
        QAbstractGraphicsShapeItem::hoverEnterEvent(event);
    }

    void NodeAttribute::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
    {
        update();
        QAbstractGraphicsShapeItem::hoverLeaveEvent(event);
    }

    NodePlug* NodeAttribute::getInputPlug() const
    {
        return m_pInputPlug;
    }

    NodePlug* NodeAttribute::getOutputPlug() const
    {
        return m_pOutputPlug;
    }

    NodeItem* NodeAttribute::getParentNode() const
    {
        return dynamic_cast<NodeItem*>(parentItem());
    }

    std::shared_ptr<core::Attribute> NodeAttribute::getAttribute() const
    {
        return m_attribute;
    }

	/*-------------------------------------------*/
    /*-----------MARK: NodeItem------------------*/
    /*-------------------------------------------*/
	NodeItem::NodeItem(std::weak_ptr<core::Node> node,
					   QGraphicsItem* parent) :
		QAbstractGraphicsShapeItem(parent),
		m_node(node),
		m_attributes()
	{
		setAcceptHoverEvents(true);
		setBrush(NodeColor);
		setPen(QPen(NodeBorderColor, 4));

		if (auto nodePtr = m_node.lock())
		{
			std::println("Node: {}", nodePtr->getName());
			NodeNameBase* NodeName = new NodeNameBase(QString::fromStdString(nodePtr->getName()), this);
			NodeName->setZValue(1);

			uint32_t inputYOffset = 55;
			auto attributes = nodePtr->getAttributes();
			auto isOutput = [](std::shared_ptr<core::Attribute> attribute) { return attribute->isReadable(); };
			auto isInput = [](std::shared_ptr<core::Attribute> attribute) { return attribute->isWritable(); };

			// for (auto outputAttribute : attributes | std::views::filter(isOutput))
			//{
			//	NodeAttribute* attribute = new NodeAttribute(outputAttribute, this);
			//	attribute->setZValue(1);
			//	attribute->setPos(-10, inputYOffset);
			//	m_attributes.push_back(attribute);
			//	inputYOffset += 29;
			// }

			// TODO output should be added first

			for (auto inputAttribute : attributes)
			{
				NodeAttribute* attribute = new NodeAttribute(inputAttribute, this);
				attribute->setZValue(1);
				attribute->setPos(-10, inputYOffset);
				m_attributes.push_back(attribute);
				inputYOffset += 29;
			}
		}
	}

    QRectF NodeItem::boundingRect() const
    {
       	return QRectF(0, 0, NodeWidth, NodeHeight);
    }

    void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setBrush(brush());
        painter->setPen(pen());
        painter->drawRoundedRect(0, 0, NodeWidth, NodeHeight, 20, 20);
    }

    void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
    {
        //TODO implement
        QAbstractGraphicsShapeItem::hoverEnterEvent(event);
    }

    void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
    {
        //TODO implement
        QAbstractGraphicsShapeItem::hoverLeaveEvent(event);
    }

    std::vector<NodeAttribute*> NodeItem::getAttributes() const
    {
        return m_attributes;
    }

    std::weak_ptr<core::Node> NodeItem::getNode() const
    {
        return m_node;
    }


	/*-------------------------------------------*/
	/*-----------MARK: NodeConnection------------*/
	/*-------------------------------------------*/
    NodeConnection::NodeConnection(std::weak_ptr<core::Connection> connection, QGraphicsItem* parent) :
        QAbstractGraphicsShapeItem(parent),
        m_connection(connection),
        m_startPos(),
        m_endPos()
    {
    }

    void NodeConnection::initialize()
	{
        //TODO simplify
		if (!scene())
		{
			std::println("No scene");
		}

		for (auto item : scene()->items())
		{
			if (auto nodeItem = dynamic_cast<NodeItem*>(item))
			{
				// Find Output node
				if (auto node = nodeItem->getNode().lock())
				{
					// Find Source node
					if (node == m_connection.lock()->sourceNode) //
					{
						// Find source attribute
						for (auto attribute : nodeItem->getAttributes())
						{
							if (attribute->getAttribute() == m_connection.lock()->sourceAttrName)
							{
								NodePlug* plug = attribute->getOutputPlug();
								plug->setConnected(true);
								m_startPos = plug->getPlugCenterPosition();
							}
						}
					}

					// Find Target node
					if (node == m_connection.lock()->targetNode)
					{
						// Find target attribute
						for (auto attribute : nodeItem->getAttributes())
						{
							if (attribute->getAttribute() == m_connection.lock()->targetAttrName)
							{
								NodePlug* plug = attribute->getInputPlug();
								plug->setConnected(true);
								m_endPos = plug->getPlugCenterPosition();
							}
						}
					}
				}
			}
		}
	}

    QRectF NodeConnection::boundingRect() const
    {
        return QRectF(m_startPos, m_endPos);
    }

    void NodeConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		Q_UNUSED(option);
		Q_UNUSED(widget);

		painter->setPen(QPen(nodeConnectionColor, 3));

		// Create a QPainterPath starting at m_startPos
		QPainterPath path(m_startPos);

		// Calculate midpoints for control points
		QPointF midPoint1 = QPointF((m_startPos.x() + m_endPos.x()) / 2, m_startPos.y());
		QPointF midPoint2 = QPointF((m_startPos.x() + m_endPos.x()) / 2, m_endPos.y());

		// Depending on the relative position of start and end, adjust the control points
		if (m_startPos.y() < m_endPos.y())
		{
			// For a downward "S" shape
			path.cubicTo(midPoint1, QPointF(midPoint1.x(), midPoint1.y() + (m_endPos.y() - m_startPos.y()) / 3), QPointF((m_startPos.x() + m_endPos.x()) / 2, (m_startPos.y() + m_endPos.y()) / 2));
			path.cubicTo(QPointF(midPoint2.x(), midPoint2.y() - (m_endPos.y() - m_startPos.y()) / 3), midPoint2, m_endPos);
		}
		else
		{
			// For an upward "S" shape
			path.cubicTo(midPoint1, QPointF(midPoint1.x(), midPoint1.y() - (m_startPos.y() - m_endPos.y()) / 3), QPointF((m_startPos.x() + m_endPos.x()) / 2, (m_startPos.y() + m_endPos.y()) / 2));
			path.cubicTo(QPointF(midPoint2.x(), midPoint2.y() + (m_startPos.y() - m_endPos.y()) / 3), midPoint2, m_endPos);
		}

		// Draw the path
		painter->drawPath(path);
	}

	/*-------------------------------------------*/
	/*-----------MARK: NodeScene-----------------*/
	/*-------------------------------------------*/
	NodeScene::NodeScene(QObject* parent) :
		QGraphicsScene(parent),
		currentLineStart(),
		currentLineEnd(),
		m_state(State::eIdle)
	{
		setSceneRect(-sceneWidth, -sceneHeight, 2 * sceneWidth, 2 * sceneHeight);
	}

	void NodeScene::drawBackground(QPainter* painter, const QRectF& rect)
	{
		Q_UNUSED(rect);

		// Draw grid lines
		painter->setPen(Qt::lightGray);
		for (int32_t x = -sceneWidth; x <= sceneWidth; x += 10)
		{
			painter->drawLine(x * 10, -sceneHeight, x * 10, sceneHeight);
		}

		for (int32_t y = -sceneHeight; y <= sceneHeight; y += 10)
		{
			painter->drawLine(-sceneWidth, y * 10, sceneWidth, y * 10);
		}

		// Draw axis
		painter->setPen(QPen(Qt::black, 2));
		painter->drawLine(-sceneWidth, 0, sceneWidth, 0);
		painter->drawLine(0, -sceneHeight, 0, sceneHeight);
	}

    void NodeScene::setNodeGraph(core::NodeGraph* nodeGraph)
    {
        m_nodeGraph = nodeGraph;
    }

    void NodeScene::updateScene()
    {
        clear();

        if (m_nodeGraph)
        {
            // Add nodes
            for (const auto& node : m_nodeGraph->getNodes())
            {
                addNode(node);
            }

            // Add connections
            for (const auto& connection : m_nodeGraph->getConnections())
            {
                addConnection(connection);
            }
        }
    }

    void NodeScene::addNode(std::weak_ptr<core::Node> node)
    {
		// Check if the node is already in the scene
		if (m_nodes.find(node) != m_nodes.end())
		{
			return;
		}

		// TODO refactor placment of nodes
		static int32_t nodeXPosition = 100;
		// auto nodeItem = new NodeItem(node);
		// addItem(nodeItem);
		NodeItem* nodeItem = new NodeItem(node);
		nodeItem->setPos(nodeXPosition, -100);
		addItem(nodeItem);

		m_nodes[node] = nodeItem;

		nodeXPosition += 500;
	}

    void removeNode(std::weak_ptr<core::Node> node)
    {

    }

    void NodeScene::addConnection(std::weak_ptr<core::Connection> connection)
    {
        NodeConnection* connectionItem = new NodeConnection(connection);
        addItem(connectionItem);
        connectionItem->initialize();
    }

    void NodeScene::removeConnection(std::weak_ptr<core::Connection> connection)
    {
        //TODO implement
    }

    /*------------------------------------------*/
    /*-----------Event Handlers-----------------*/
    /*------------------------------------------*/
	void NodeScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
	{
		if (QGraphicsItem* item = itemAt(event->scenePos(), QTransform()))
		{
			if (auto nodePlug = dynamic_cast<NodePlug*>(item)) // TODO limit to only left click
			{
				onConnectionStart(nodePlug);
			}

			update();
		}

		QGraphicsScene::mousePressEvent(event);
	}

	void NodeScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		if (m_state == State::eConnecting)
		{
			currentLineEnd = event->scenePos();
			m_pTempConnection->updatePosition(currentLineEnd);

			if (!items().contains(m_pTempConnection)) // TODO check if it is needed
			{
				addItem(m_pTempConnection);
			}

			update();
		}

		QGraphicsScene::mouseMoveEvent(event);
	}

	void NodeScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
	{
		if (m_state == State::eConnecting)
		{
			QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
			if (auto nodePlug = dynamic_cast<NodePlug*>(item))
			{
				m_pTempConnection->finalizeConnection(nodePlug);

				NodePlug* sourcePlug = m_pTempConnection->getSourcePlug();
				NodePlug* targetPlug = m_pTempConnection->getTargetPlug();
				NodeItem* sourceNode = sourcePlug->getParentAttribute()->getParentNode();
				NodeItem* targetNode = targetPlug->getParentAttribute()->getParentNode();

				//TODO add connection to node graph
				update();
			}
			else
			{
                //TODO refactor temporary connection
				removeItem(m_pTempConnection);
				delete m_pTempConnection;
				m_pTempConnection = nullptr;
				update();
			}

			m_state = State::eIdle;
		}

		QGraphicsScene::mouseReleaseEvent(event);
	}

	/*------------------------------------*/
	/*---------MARK:NodeEditor------------*/
	/*------------------------------------*/
	NodeEditor::NodeEditor(core::ContentManagerHandler contentManager, QWidget* parent) :
		QGraphicsView(parent),
		m_scene(new NodeScene(this)),
		m_contentManager(contentManager),
		m_panning(false),
		m_lastPanPoint(QPoint(0, 0))
	{

		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		setScene(m_scene);
		centerOn(0, 0);
	}

	void NodeEditor::initialize()
	{
        m_scene->setNodeGraph(&m_contentManager->getMainNodeGraph());
	}

	/*--------------------------------*/
	/*---------Event Handlers---------*/
	/*--------------------------------*/
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

	void NodeEditor::showEvent(QShowEvent* event)
	{
		m_scene->updateScene();
		QGraphicsView::showEvent(event);
	}

} // namespace st::ui
