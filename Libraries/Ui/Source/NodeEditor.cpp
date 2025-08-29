#include "NodeEditor.hpp"
#include "Core/ContentManager.hpp"
#include "Core/Node.hpp"
#include "Core/NodeGraph.hpp"
#include "Core/EventRegistry.hpp"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QTimer>
#include <print>

#include <spdlog/spdlog.h>

namespace st::ui
{
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
	NodeItem::NodeItem(std::shared_ptr<core::Node> node,
					   QGraphicsItem* parent) :
		QAbstractGraphicsShapeItem(parent),
		m_node(node),
		m_attributes(),
		m_isSelected(false)
	{
		setAcceptHoverEvents(true);
		setFlags(flags() | QGraphicsItem::ItemIsSelectable);
		setBrush(NodeColor);
		setPen(QPen(NodeBorderColor, 4));


		std::println("Node: {}", m_node->getName());
		//m_nodeName = new NodeNameBase(QString::fromStdString(nodePtr->getName()), this);
		//m_nodeName->setZValue(1);


		uint32_t inputYOffset = 55;
		auto attributes = m_node->getAttributes();
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

    QRectF NodeItem::boundingRect() const
    {
       	return QRectF(0, 0, NodeWidth, NodeHeight);
    }

    void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
		Q_UNUSED(option);
		Q_UNUSED(widget);
	
		QColor borderColor = m_isSelected || m_isHovered ? NodeHighlightBorderColor : NodeBorderColor;
		QPen borderPen(borderColor, 4);
		
		// 1. Draw the main body background (excluding header area)
		painter->setPen(Qt::NoPen);
		painter->setBrush(NodeColor);
		QPainterPath bodyPath;
		bodyPath.addRoundedRect(QRectF(0, HeaderHeight - 20, NodeWidth, NodeHeight - HeaderHeight + 20), 20, 20);
		painter->drawPath(bodyPath);
		
		// 2. Draw the header background
		painter->setBrush(borderColor);
		painter->drawRoundedRect(QRectF(0, 0, NodeWidth, HeaderHeight), 20, 20);
		painter->drawRect(QRectF(0, 20, NodeWidth, 25)); // Bottom part of header
		
		// 3. Draw the outline for the entire node
		painter->setPen(borderPen);
		painter->setBrush(Qt::NoBrush);
		QPainterPath outlinePath;
		outlinePath.addRoundedRect(QRectF(0, 0, NodeWidth, NodeHeight), 20, 20);
		painter->drawPath(outlinePath);
		
		// 4. Draw the node name last so it's always on top

		painter->setPen(Qt::white);
		painter->setFont(QFont("Inter", 24));
		QRectF textRect(20, 10, 280, 35);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, 
							QString::fromStdString(m_node->getName()));

			
		//Draw brown circle if node is dirty
		if (m_node->isDirty())
		{
			painter->setBrush(Qt::darkRed);
			painter->drawEllipse(QPointF(280, 10), 5, 5);
		}

    }

    void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
    {
        QAbstractGraphicsShapeItem::hoverEnterEvent(event);

        if(m_isSelected)
		{
			return;
		}

		m_isHovered = true;
		update();
    }

    void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
    {
        QAbstractGraphicsShapeItem::hoverLeaveEvent(event);

       	if (m_isSelected)
		{
			return;
		}

		m_isHovered = false;
		update();
    }
	
	void NodeItem::setSelected(bool state)
	{
		m_isSelected = state;
		update();
	}

    std::vector<NodeAttribute*> NodeItem::getAttributes() const
    {
        return m_attributes;
    }

    std::shared_ptr<core::Node> NodeItem::getNode() const
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
				if (auto node = nodeItem->getNode())
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
	NodeScene::NodeScene(core::ContentManagerHandler contentManager, QObject* parent) :
		QGraphicsScene(parent),
		m_contentManager(contentManager),
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
		painter->setPen(QPen(Qt::black, 4));
		painter->drawLine(-sceneWidth, 0, sceneWidth, 0);
		painter->drawLine(0, -sceneHeight, 0, sceneHeight);
	}

    void NodeScene::setNodeGraph(core::NodeGraph* nodeGraph)
    {
        m_nodeGraph = nodeGraph;
    }

    void NodeScene::updateScene()
    {
        //clear();

        if (m_nodeGraph)
        {
			spdlog::info("NodeScene::updateScene() - Updating scene with node graph size: {}",
				m_nodeGraph->getNodes().size());
            // Add nodes
            for (const auto& node : m_nodeGraph->getNodes())
            {
				spdlog::info("NodeScene::updateScene() - Adding node: {}", node->getName());
                addNode(node);
            }

            // Add connections
            for (const auto& connection : m_nodeGraph->getConnections())
            {
                addConnection(connection);
            }
        }
		
    }

    void NodeScene::addNode(std::shared_ptr<core::Node> node)
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

    void removeNode(std::shared_ptr<core::Node> node)
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

		if (event->button() == Qt::LeftButton) {
			spdlog::warn("NodeScene::Left button pressed");
			QGraphicsItem* clickedItem = itemAt(event->scenePos(), QTransform());
			spdlog::warn("Clicked item: {}", clickedItem ? "Found" : "Not found");
			
			// Check if we clicked on a node or part of a node
			NodeItem* clickedNode = nullptr;
			
			if (clickedItem) {
				// Check if we clicked directly on a NodeItem
				clickedNode = dynamic_cast<NodeItem*>(clickedItem);
				
				// If not, check if we clicked on a child of a NodeItem
				if (!clickedNode) {
					QGraphicsItem* parent = clickedItem->parentItem();
					while (parent && !clickedNode) {
						clickedNode = dynamic_cast<NodeItem*>(parent);
						if (!clickedNode) {
							parent = parent->parentItem();
						}
					}
				}
			}

			spdlog::warn("Clicked node: {}", clickedNode ? "Found" : "Not found");
			if (clickedNode) {
				selectNode(clickedNode);
			} else {
				// We clicked on the background, deselect current node
				deselectCurrentNode();
			}
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

				[[maybe_unused]] NodePlug* sourcePlug = m_pTempConnection->getSourcePlug();
				[[maybe_unused]] NodePlug* targetPlug = m_pTempConnection->getTargetPlug();
				[[maybe_unused]] NodeItem* sourceNode = sourcePlug->getParentAttribute()->getParentNode();
				[[maybe_unused]] NodeItem* targetNode = targetPlug->getParentAttribute()->getParentNode();

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

	void NodeScene::selectNode(NodeItem* nodeItem)
	{
		if(m_pSelectedNode && m_pSelectedNode != nodeItem)
		{
			deselectCurrentNode();
		}

		m_pSelectedNode = nodeItem;
		if (nodeItem)
		{
			spdlog::warn("NodeItem: {} is selected", nodeItem->getNode()->getName());
			nodeItem->setSelected(true);
		}

		m_contentManager->updateSelection(nodeItem->getNode());
	}

	void NodeScene::deselectCurrentNode()
	{
		if (m_pSelectedNode)
		{
			m_pSelectedNode->setSelected(false);
			m_pSelectedNode = nullptr;
		}
	}

	NodeItem* NodeScene::getSelectedNode() const
	{
		return m_pSelectedNode;
	}

	/*------------------------------------*/
	/*---------MARK:NodeEditor------------*/
	/*------------------------------------*/
	NodeGraphView::NodeGraphView(core::ContentManagerHandler contentManager, QWidget* parent) :
		QGraphicsView(parent),
		m_scene(new NodeScene(contentManager, this)),
		m_contentManager(contentManager),
		m_panning(false),
		m_lastPanPoint(QPoint(0, 0))
	{

		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		setScene(m_scene);


		QTimer::singleShot(0, this, [this]() {
			centerOn(0, 0);
		});

	}

	/*--------------------------------*/
	/*---------Event Handlers---------*/
	/*--------------------------------*/
	void NodeGraphView::mousePressEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::MiddleButton)
		{
			m_panning = true;
			m_lastPanPoint = event->position();
			setCursor(Qt::ClosedHandCursor);
		}

		QGraphicsView::mousePressEvent(event);
	}

	void NodeGraphView::mouseMoveEvent(QMouseEvent* event)
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

	void NodeGraphView::mouseReleaseEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::MiddleButton)
		{
			m_panning = false;
			setCursor(Qt::ArrowCursor);
		}

		QGraphicsView::mouseReleaseEvent(event);
	}

	void NodeGraphView::wheelEvent(QWheelEvent* event)
	{
		double angleDeltaY = event->angleDelta().y();

		double factor = std::pow(1.0015, angleDeltaY);

		scale(factor, factor);

		QGraphicsView::wheelEvent(event);
	}

	void NodeGraphView::resizeEvent(QResizeEvent* event)
	{
		QGraphicsView::resizeEvent(event);
	}

	void NodeGraphView::showEvent(QShowEvent* event)
	{
		QGraphicsView::showEvent(event);

		m_scene->setNodeGraph(&m_contentManager->getMainNodeGraph());
		m_scene->updateScene();
		centerOn(0, 0);

		viewport()->update();
	}

	/*------------------------------------*/
	/*---------MARK:NodeEditorWidget------*/
	/*------------------------------------*/
	NodeEditor::NodeEditor(core::ContentManagerHandler contentManager, QWidget* parent) :
		QWidget(parent),
		m_nodeEditor(nullptr),
		m_contentManager(contentManager)
	{
		QVBoxLayout* layout = new QVBoxLayout(this);
		QHBoxLayout* toolboxLayout = new QHBoxLayout(this);

		QCheckBox* evaluateGraph = new QCheckBox("Enable Graph Evaluation", this);
		evaluateGraph->setChecked(true);
		connect(evaluateGraph, &QCheckBox::checkStateChanged, this, &NodeEditor::onEvaluateGraphStateChanged);
		
		QCheckBox* showGrid = new QCheckBox("Show Grid", this);
		showGrid->setChecked(true);

		
		toolboxLayout->addWidget(evaluateGraph);
		toolboxLayout->addWidget(showGrid);
		toolboxLayout->setContentsMargins(0, 0, 0, 0);
		toolboxLayout->setSpacing(0);
		toolboxLayout->setAlignment(Qt::AlignLeft);



		QHBoxLayout* nodeEditorLayout = new QHBoxLayout(this);
		m_nodeEditor = new NodeGraphView(contentManager, this);
		nodeEditorLayout->addWidget(m_nodeEditor);

		layout->addLayout(toolboxLayout);
		layout->addLayout(nodeEditorLayout);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);
		setLayout(layout);
	}

	void NodeEditor::refreshNodeGraph()
	{
		if (m_nodeEditor)
		{
			NodeScene* scene = dynamic_cast<NodeScene*>(m_nodeEditor->scene());
			if (scene)
			{
				spdlog::info("Refreshing NodeGraphView");
				scene->setNodeGraph(&m_contentManager->getMainNodeGraph());
				scene->updateScene();
				m_nodeEditor->centerOn(0,0);
			}
			else
			{
				spdlog::error("NodeGraphView does not have a valid NodeScene");
				throw std::runtime_error("NodeGraphView does not have a valid NodeScene");
			}

		}
	}

	void NodeEditor::onEvaluateGraphStateChanged(int state)
	{
		auto& nodeGraph = m_contentManager->getMainNodeGraph();

		if (state == Qt::Checked)
		{
			spdlog::info("NodeGraph evaluation enabled");
			nodeGraph.enableEvaluation();
		}
		else
		{
			spdlog::info("NodeGraph evaluation disabled");
			nodeGraph.disableEvaluation();
		}
	}


} // namespace st::ui
