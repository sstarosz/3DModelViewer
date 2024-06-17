#ifndef ST_UI_NODEEDITOR_HPP
#define ST_UI_NODEEDITOR_HPP

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsSceneMouseEvent>

#include "Core/NodeGraph.hpp"
#include "Core/Nodes/Node.hpp"

#include <print>
#include <algorithm>
#include <ranges>

namespace st
{

	namespace core
	{
		class ContentManager;
	} // namespace core

	namespace ui
	{
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
				painter->drawRoundedRect(0, 0, 15, 25, 5, 5);

				int32_t drawRect = isUnderMouse() ? 20 : 10;
				painter->drawRect(5, 0, drawRect, 25);

				painter->drawRect(20, 0, 5, 25);
			}

			void setConnected(bool state)
			{
				m_isConnected = state;
			}

			bool isConnected() const
			{
				return m_isConnected;
			}

		  private:
			bool m_isHovered = false;
			bool m_isConnected = false;
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
			static constexpr int32_t nodeLabelWidth = 270;
			static constexpr int32_t nodeLabelHeight = 25;

			// Colors
			//#585F63
			static constexpr QColor nodeLabelColor = QColor(88, 95, 99);
			//#87E5CF
			static constexpr QColor nodeConnectionColor = QColor(135, 229, 207);

		  public:
			NodeAttribute(core::Handler handler, QGraphicsItem* parent = nullptr) :
				QAbstractGraphicsShapeItem(parent),
				m_handler(handler)
			{
				setAcceptHoverEvents(true);

				setBrush(nodeLabelColor);
				setPen(Qt::NoPen);


				//Draw connection Input
				if(m_handler.isWritable())
				{
					NodePlug* nodePlug = new NodePlug(this);
					nodePlug->setPos(0, 0);
					m_nodePlugs.push_back(nodePlug);
				}

				//Draw Connection Output
				if(m_handler.isReadable())
				{
					NodePlug* nodePlug = new NodePlug(this);
					QTransform transform;
					transform.translate(320, 0);
					transform.scale(-1, 1);
					nodePlug->setTransform(transform);
					m_nodePlugs.push_back(nodePlug);
				}

			}

			QRectF boundingRect() const override
			{
				return QRectF(0, 0, nodeWidth, nodeHeight);
			}

			void paint(QPainter* painter,
					   [[maybe_unused]]const QStyleOptionGraphicsItem* option,
					   [[maybe_unused]]QWidget* widget) override
			{
				//TODO color of connection that depend on type of the handler
				//TODO connection and disconnection animation
				

				//Draw Label
				painter->setPen(pen());
				painter->setBrush(brush());
				painter->drawRect(25,
								  0,
								  nodeLabelWidth,
								  nodeLabelHeight);


				// Draw text
				painter->setPen(Qt::white);
				painter->setFont(QFont("Inter", 12));
				QRectF textRect{45, 5, 230, 15};
				painter->drawText(textRect,
								  Qt::AlignLeft | Qt::AlignVCenter,
								  QString::fromStdString(m_handler.getName()));


				//If int or float
				//Create input bar



				//If bool
				//Create switch

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

			std::vector<NodePlug*> getPlugs() const
			{
				return m_nodePlugs;
			}

		private:
			core::Handler m_handler; // Handler to Attribute
			std::vector<NodePlug*> m_nodePlugs;
		};

		class NodeItem : public QAbstractGraphicsShapeItem
		{
		  public:
			NodeItem(std::weak_ptr<core::Node2> node,
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
					auto isOutput = [](core::Handler handler){ return handler.isReadable();};
					auto isInput = [](core::Handler handler){ return handler.isWritable();};

					for (auto outputAttribute : attributes | std::views::filter(isOutput))
					{
						NodeAttribute* attribute = new NodeAttribute(outputAttribute, this);
						attribute->setZValue(1);
						attribute->setPos(-10, inputYOffset);
						m_attributes.push_back(attribute);
						inputYOffset += 29;
					}

					for (auto inputAttribute : attributes | std::views::filter(isInput))
					{
						NodeAttribute* attribute = new NodeAttribute(inputAttribute, this);
						attribute->setZValue(1);
						attribute->setPos(-10, inputYOffset);
						m_attributes.push_back(attribute);
						inputYOffset += 29;
					}
				}
			}

			QRectF boundingRect() const override
			{
				return QRectF(0, 0, 320, 400);
			}

			void paint(QPainter* painter,
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
				//setPen(QPen(Qt::red, 4));
				//update();
				QAbstractGraphicsShapeItem::hoverEnterEvent(event);
			}

			void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override
			{
				//setPen(QPen(NodeBorderColor, 4));
				//update();
				QAbstractGraphicsShapeItem::hoverLeaveEvent(event);
			}

			std::vector<NodeAttribute*> getAttributes() const
			{
				return m_attributes;
			}

		  private:
			std::weak_ptr<core::Node2> m_node;
			std::vector<NodeAttribute*> m_attributes;
			QString m_nodeName;

			static constexpr int32_t NodeWidth = 100;
			static constexpr int32_t NodeHeight = 100;
		};

		class PlugConnection : public QAbstractGraphicsShapeItem
		{
			static constexpr QColor nodeConnectionColor = QColor(85, 209, 208);
			
		public:

			explicit PlugConnection(NodePlug* startPlug, NodePlug* endPlug, QGraphicsItem* parent = nullptr) :
				QAbstractGraphicsShapeItem(parent),
				m_startPlug(startPlug),
				m_endPlug(endPlug)
			{
				setAcceptHoverEvents(true);
			}

			QRectF boundingRect() const override
			{
				return QRectF(0, 0, 0, 0);
			}

			void paint(QPainter* painter,
					   const QStyleOptionGraphicsItem* option,
					   QWidget* widget) override
			{
				Q_UNUSED(option);
				Q_UNUSED(widget);

				//TODO change line to bezier curve
				painter->setPen(QPen(nodeConnectionColor, 3));
				painter->drawLine(m_startPlug->scenePos() + QPointF(0.0f, 12.5f), m_endPlug->scenePos() + QPointF(0.0f, 12.5f));
			}


			private:
			NodePlug* m_startPlug;
			NodePlug* m_endPlug;
		};

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

			void addNode(std::weak_ptr<core::Node2> node)
			{
				// auto nodeItem = new NodeItem(node);
				// addItem(nodeItem);
			}

			void removeNode(std::weak_ptr<core::Node2> node)
			{
			}

			void drawBackground(QPainter* painter, const QRectF& rect) override;

			void drawForeground(QPainter* painter, const QRectF& rect) override
			{
				Q_UNUSED(rect);
				Q_UNUSED(painter);

				//if(m_drawConnection)
				//{
				//	QColor connectionColor{85, 209, 208};
				//	painter->setPen(QPen(connectionColor, 3));
				//	painter->drawLine(currentLineStart, currentLineEnd);
				//}
			}

			void setView(QGraphicsView* view)
			{
				m_view = view;
			}

			void mousePressEvent(QGraphicsSceneMouseEvent* event) override
			{
				if (QGraphicsItem* item = itemAt(event->scenePos(), QTransform()))
				{
					if (auto nodePlug = dynamic_cast<NodePlug*>(item))
					{
						m_state = State::eConnecting;
						

						currentLineStart = nodePlug->scenePos() + QPointF(0.0f, 12.5f);
						currentLineEnd = currentLineStart; // Initialize end point
						plugDurningConnection = nodePlug;
					}
				}

				update();
				QGraphicsScene::mousePressEvent(event);
			}

			void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
			{
				//if (m_drawConnection)
				//{
				//	currentLineEnd = event->scenePos();
				//	update();
				//}
				//else
				//{
				//	QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
				//	if (item && dynamic_cast<NodePlug*>(item))
				//	{
				//		// Hover logic can be implemented here if needed
				//	}
				//}
				update();
				QGraphicsScene::mouseMoveEvent(event);
			}

			void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
			{
				// if (m_drawConnection)
				// {
				// 	NodePlug* endPlug = nullptr;
				// 	QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
				// 	if (item)
				// 	{
				// 		endPlug = dynamic_cast<NodePlug*>(item);
				// 	}

				// 	if (plugDurningConnection && endPlug)
				// 	{
				// 		// Create and add the connection
				// 		PlugConnection* connection = new PlugConnection(plugDurningConnection, endPlug);
				// 		addItem(connection);

				// 		plugDurningConnection->setConnected(true);
				// 		endPlug->setConnected(true);
				// 	}

				// 	m_drawConnection = false;
				// 	update();
				// }

				update();
				QGraphicsScene::mouseReleaseEvent(event);
			}

		  private:
			QGraphicsView* m_view;

			NodePlug* lastHoveredPlug = nullptr;
			NodePlug* plugDurningConnection = nullptr;

			QPointF currentLineStart;
			QPointF currentLineEnd;
			State m_state;
		};

		class NodeEditor : public QGraphicsView
		{
			Q_OBJECT

		  public:
			explicit NodeEditor(core::ContentManager& contentManager, QWidget* parent = nullptr);

			void initialize();

		  protected:
			void mousePressEvent(QMouseEvent* event) override;
			void mouseMoveEvent(QMouseEvent* event) override;
			void mouseReleaseEvent(QMouseEvent* event) override;
			void wheelEvent(QWheelEvent* event) override;

			void resizeEvent(QResizeEvent* event) override;

		  private:
			void setupScene();

			NodeScene* m_scene;
			core::ContentManager& m_contentManager;

			bool m_panning;
			QPointF m_lastPanPoint;
		};

	} // namespace ui
} // namespace st

#endif // ST_UI_NODEEDITOR_HPP
