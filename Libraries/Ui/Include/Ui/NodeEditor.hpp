#ifndef ST_UI_NODEEDITOR_HPP
#define ST_UI_NODEEDITOR_HPP

#include <QAbstractGraphicsShapeItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QShowEvent>


#include "Core/Nodes/Node.hpp"
#include "Core/Nodes/NodeGraph.hpp"
#include "Core/ContentManager.hpp"

#include <algorithm>
#include <print>
#include <ranges>

namespace st
{

	namespace core
	{
		class ContentManager;
	} // namespace core

	namespace ui
	{
		/*------------------------------------*/
		/*------------Utility-----------------*/
		/*------------------------------------*/
		struct WeakPtrHash
		{
			template <typename T>
			std::size_t operator() (std::weak_ptr<T> const& wp) const
			{
				auto sp = wp.lock(); // Convert to shared_ptr and use its standard hasher
				return std::hash<std::shared_ptr<T>>{}(sp);
			}
		};

		struct WeakPtrEqual
		{
			template <typename T>
			bool operator() (std::weak_ptr<T> const& lhs, std::weak_ptr<T> const& rhs) const
			{
				return lhs.lock() == rhs.lock(); // Compare underlying shared_ptr objects
			}
		};

		class NodePlug;
		class NodeAttribute;
		class NodeItem;
		class NodeScene;

		// #373B3E
		constexpr QColor NodeColor = QColor(55, 59, 62);
		// #5A676B
		constexpr QColor NodeBorderColor = QColor(90, 103, 107);
		// #55D1D0
		constexpr QColor NodeInputBarBorderColor = QColor(85, 209, 208);

		class NodeNameBase : public QAbstractGraphicsShapeItem
		{
		  public:
			NodeNameBase(const QString& name, QGraphicsItem* parent = nullptr) :
				QAbstractGraphicsShapeItem(parent),
				m_name(name)
			{
			}

			QRectF boundingRect() const override
			{
				return QRectF(0, 0, 300, 45);
			}

			void paint(QPainter* painter,
					   const QStyleOptionGraphicsItem* option,
					   QWidget* widget) override
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

		  private:
			QString m_name;
		};

		class NodePlug : public QAbstractGraphicsShapeItem
		{
		  private:
			static constexpr QColor nodeConnectionColor = QColor(85, 209, 208);
			static constexpr QColor nodeConnectionHover = QColor(175, 233, 233);

		  public:
			NodePlug(QGraphicsItem* parent = nullptr) :
				QAbstractGraphicsShapeItem(parent)
			{
				setAcceptHoverEvents(true);

				setBrush(nodeConnectionColor);
				setPen(Qt::NoPen);
			}

			QPainterPath createConnectionPath()
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

			QRectF boundingRect() const override
			{
				return QRectF(0.0F, 0.0F, 25.0F, 25.0F);
			}

			void paint(QPainter* painter,
					   const QStyleOptionGraphicsItem* option,
					   QWidget* widget) override
			{
				Q_UNUSED(option);
				Q_UNUSED(widget);

				painter->setPen(pen());
				painter->setBrush(brush());

				QPainterPath path = createConnectionPath();
				painter->drawPath(path);
			}

			void setConnected(bool state)
			{
				m_isConnected = state;
			}

			bool isConnected() const
			{
				return m_isConnected;
			}

			void beginConnection()
			{
				m_isConnecting = true;
			}

			void endConnection()
			{
				m_isConnecting = false;
			}

			NodeAttribute* getParentAttribute() const;

			QPointF getPlugCenterPosition() const
			{
				// TODO refactor
				return scenePos() + QPointF(0, 12.5);
			}

		  private:
			bool m_isHovered = false;
			bool m_isConnected = false;
			bool m_isConnecting = false;
		};

		//--------------------------------------------------------------------------------------------------
		// NodeAttribute
		//--------------------------------------------------------------------------------------------------
		class NodeAttribute : public QAbstractGraphicsShapeItem
		{
		  private:
			// Size
			// Main node size
			static constexpr int32_t nodeWidth = 320;
			static constexpr int32_t nodeHeight = 25;

			// Node Laber
			static constexpr int32_t nodeLabelMarginHorizontal = 25;
			static constexpr int32_t nodeLabelMarginVertical = 0;
			static constexpr int32_t nodeLabelWidth = 280;
			static constexpr int32_t nodeLabelHeight = 25;

			// Colors
			// #585F63
			static constexpr QColor nodeLabelColor = QColor(88, 95, 99);
			// #87E5CF
			static constexpr QColor nodeConnectionColor = QColor(85, 209, 208);
			;

		  public:
			NodeAttribute(std::shared_ptr<core::Attribute> attribute,
						  QGraphicsItem* parent = nullptr) :
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
					NodePlug* nodePlug = new NodePlug(this);
					QTransform transform;
					transform.translate(320, 0);
					transform.scale(-1, 1);
					nodePlug->setTransform(transform);
					m_pOutputPlug = nodePlug;
				}
			}

			virtual ~NodeAttribute() override{};

			QRectF boundingRect() const override
			{
				return QRectF(0, 0, nodeWidth, nodeHeight);
			}

			void paint(QPainter* painter,
					   [[maybe_unused]] const QStyleOptionGraphicsItem* option,
					   [[maybe_unused]] QWidget* widget) override
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

			void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override
			{
				update();
				QAbstractGraphicsShapeItem::hoverEnterEvent(event);
			}

			void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override
			{
				update();
				QAbstractGraphicsShapeItem::hoverLeaveEvent(event);
			}

			NodePlug* getInputPlug() const
			{
				return m_pInputPlug;
			}

			NodePlug* getOutputPlug() const
			{
				return m_pOutputPlug;
			}

			NodeItem* getParentNode() const;

			std::shared_ptr<core::Attribute> getAttribute() const
			{
				return m_attribute;
			}

		  private:
			std::shared_ptr<core::Attribute> m_attribute; // Handler to Attribute
			NodePlug* m_pInputPlug;
			NodePlug* m_pOutputPlug;
		};

		class NodeItem : public QAbstractGraphicsShapeItem
		{
		  public:
			NodeItem(std::weak_ptr<core::Node> node,
					 QGraphicsItem* parent = nullptr) :
				QAbstractGraphicsShapeItem(parent),
				m_node(node)
			{
				setAcceptHoverEvents(true);
				setBrush(NodeColor);
				setPen(QPen(NodeBorderColor, 4));

				if (auto nodePtr = m_node.lock())
				{
					m_nodeName = QString::fromStdString(nodePtr->getName());
					std::println("Node: {}", nodePtr->getName());
					NodeNameBase* NodeName = new NodeNameBase(m_nodeName, this);
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

			virtual ~NodeItem() override{};

			virtual QRectF boundingRect() const override
			{
				return QRectF(0, 0, 320, 400);
			}

			virtual void paint(QPainter* painter,
							   const QStyleOptionGraphicsItem* option,
							   QWidget* widget) override
			{
				Q_UNUSED(option);
				Q_UNUSED(widget);

				// Draw black
				painter->setPen(pen());
				painter->setBrush(brush());
				painter->drawRoundedRect(0, 0, 300, 400, 20, 20);
			}

			void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override
			{
				// setPen(QPen(Qt::red, 4));
				// update();
				QAbstractGraphicsShapeItem::hoverEnterEvent(event);
			}

			void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override
			{
				// setPen(QPen(NodeBorderColor, 4));
				// update();
				QAbstractGraphicsShapeItem::hoverLeaveEvent(event);
			}

			std::vector<NodeAttribute*> getAttributes() const
			{
				return m_attributes;
			}

			std::weak_ptr<core::Node> getNode() const
			{
				return m_node;
			}

		  private:
			std::weak_ptr<core::Node> m_node;
			std::vector<NodeAttribute*> m_attributes;
			QString m_nodeName;

			static constexpr int32_t NodeWidth = 100;
			static constexpr int32_t NodeHeight = 100;
		};

		class PlugConnection : public QAbstractGraphicsShapeItem
		{
			static constexpr QColor nodeConnectionColor = QColor(85, 209, 208);

			// Position of the plug
			static constexpr QPointF plugPosition = QPointF(0.0f, 12.5f);

		  public:
			explicit PlugConnection(NodePlug* startPlug,
									QGraphicsItem* parent = nullptr) :
				QAbstractGraphicsShapeItem(parent),
				m_startPos(startPlug->scenePos() + plugPosition),
				m_endPos(startPlug->scenePos() + plugPosition),
				m_startPlug(startPlug),
				m_endPlug(nullptr)
			{
				setAcceptHoverEvents(true);
			}

			PlugConnection(NodePlug* startPlug,
						   NodePlug* endPlug,
						   QGraphicsItem* parent = nullptr) :
				QAbstractGraphicsShapeItem(parent),
				m_startPos(startPlug->scenePos() + plugPosition),
				m_endPos(startPlug->scenePos() + plugPosition),
				m_startPlug(startPlug),
				m_endPlug(endPlug)
			{
				setAcceptHoverEvents(true);
				finalizeConnection(endPlug);
			}

			QRectF boundingRect() const override
			{
				return QRectF(m_startPos, m_endPos);
			}

			void paint(QPainter* painter,
					   const QStyleOptionGraphicsItem* option,
					   QWidget* widget) override
			{
				Q_UNUSED(option);
				Q_UNUSED(widget);

				// TODO change line to bezier curve
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

			void updatePosition(QPointF endPos)
			{
				m_endPos = endPos;

				prepareGeometryChange();
			}

			void finalizeConnection(NodePlug* endPlug)
			{
				m_endPos = endPlug->scenePos() + plugPosition;
				m_endPlug = endPlug;

				m_startPlug->endConnection();
				m_endPlug->endConnection();

				m_startPlug->setConnected(true);
				m_endPlug->setConnected(true);
			}

			NodePlug* getSourcePlug() const
			{
				return m_startPlug;
			}

			NodePlug* getTargetPlug() const
			{
				return m_endPlug;
			}

		  private:
			QPointF m_startPos;
			QPointF m_endPos;
			NodePlug* m_startPlug;
			NodePlug* m_endPlug;
		};

		class NodeConnection : public QAbstractGraphicsShapeItem
		{
			static constexpr QColor nodeConnectionColor = QColor(85, 209, 208);

			// Position of the plug
			static constexpr QPointF plugPosition = QPointF(0.0f, 12.5f);

		  public:
			NodeConnection(std::weak_ptr<core::Connection> connection,
						   QGraphicsItem* parent = nullptr) :
				QAbstractGraphicsShapeItem(parent),
				m_connection(connection)
			{
			}

			void initialize()
			{
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

			QRectF boundingRect() const override
			{
				return QRectF(0, 0, 100, 100);
			}

			void paint(QPainter* painter,
					   const QStyleOptionGraphicsItem* option,
					   QWidget* widget) override
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

		  private:
			std::weak_ptr<core::Connection> m_connection;
			QPointF m_startPos;
			QPointF m_endPos;
		};

		inline NodeItem* NodeAttribute::getParentNode() const
		{
			return dynamic_cast<NodeItem*>(parentItem());
		}

		inline NodeAttribute* NodePlug::getParentAttribute() const
		{
			return dynamic_cast<NodeAttribute*>(parentItem());
		}

		/**
		 * @brief Represent node graph in the scene
		 *
		 */
		class NodeScene : public QGraphicsScene
		{
			Q_OBJECT

			enum class State
			{
				eUndefined,
				eIdle,
				eConnecting,
				eNrOfStates
			};

		  public:
			explicit NodeScene(QObject* parent = nullptr);

			void setNodeGraph(core::NodeGraphHandler nodeGraph)
			{
				m_nodeGraph = nodeGraph;
			}

			void updateScene()
			{
				// TODO
				// Check if the node is already in the scene
				// If not add it
				for (auto node : m_nodeGraph->getNodes())
				{
					addNode(node);
				}

				// TODO
				std::println("Connections: {}", m_nodeGraph->getConnections().size());
				for (auto connection : m_nodeGraph->getConnections())
				{
					// TODO
					addConnection(connection);
				}
			}

			void addNode(std::weak_ptr<core::Node> node)
			{
				// Check if the node is already in the scene
				if(m_nodes.find(node) != m_nodes.end())
				{
					return;
				}
				
				
				// TODO refactor
				static int32_t nodeXPosition = 100;
				// auto nodeItem = new NodeItem(node);
				// addItem(nodeItem);
				NodeItem* nodeItem = new NodeItem(node);
				nodeItem->setPos(nodeXPosition, -100);
				addItem(nodeItem);

				m_nodes[node] = nodeItem;

				nodeXPosition += 500;
			}

			void addConnection(std::weak_ptr<core::Connection> connection)
			{
				NodeConnection* connectionItem = new NodeConnection(connection);
				addItem(connectionItem);
				connectionItem->initialize();
			}

			void removeNode(std::weak_ptr<core::Node> node)
			{
			}

			void drawBackground(QPainter* painter, const QRectF& rect) override;

			void onConnectionStart(NodePlug* plug)
			{
				m_state = State::eConnecting;
				plug->beginConnection();
				m_pTempConnection = new PlugConnection(plug, plug);
			}

			void mousePressEvent(QGraphicsSceneMouseEvent* event) override
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

			void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
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

			void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
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

						// m_nodeGraph.addConnection(
						//	sourceNode->getNode(),
						//	sourcePlug->getParentAttribute()->getHandler(),
						//	targetNode->getNode(),
						//	targetPlug->getParentAttribute()->getHandler()
						//)
						//
						//
						// m_connections.push_back(m_pTempConnection);
						update();
					}
					else
					{
						removeItem(m_pTempConnection);
						delete m_pTempConnection;
						m_pTempConnection = nullptr;
						update();
					}

					m_state = State::eIdle;
				}

				QGraphicsScene::mouseReleaseEvent(event);
			}

		  private:
			core::NodeGraphHandler m_nodeGraph;
			std::unordered_map<std::weak_ptr<core::Node>, NodeItem*, WeakPtrHash, WeakPtrEqual> m_nodes;

			PlugConnection* m_pTempConnection = nullptr;
			std::vector<PlugConnection*> m_connections;

			QPointF currentLineStart;
			QPointF currentLineEnd;
			State m_state;
		};

		class NodeEditor : public QGraphicsView
		{
			Q_OBJECT

		  public:
			explicit NodeEditor(core::ContentManagerHandler contentManager, QWidget* parent = nullptr);

			void initialize();

		  protected:
			void mousePressEvent(QMouseEvent* event) override;
			void mouseMoveEvent(QMouseEvent* event) override;
			void mouseReleaseEvent(QMouseEvent* event) override;
			void wheelEvent(QWheelEvent* event) override;

			void resizeEvent(QResizeEvent* event) override;
			void showEvent(QShowEvent* event) override;

		  private:
			void setupScene();

			NodeScene* m_scene;
			core::ContentManagerHandler m_contentManager;

			bool m_panning;
			QPointF m_lastPanPoint;
		};

	} // namespace ui
} // namespace st

#endif // ST_UI_NODEEDITOR_HPP
