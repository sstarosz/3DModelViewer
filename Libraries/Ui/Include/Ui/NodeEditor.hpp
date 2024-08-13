#ifndef ST_UI_NODEEDITOR_HPP
#define ST_UI_NODEEDITOR_HPP

#include <QAbstractGraphicsShapeItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainterPath>
#include <QShowEvent>
#include <QStyleOptionGraphicsItem>

#include "Core/ContentManager.hpp"
#include "Core/Node.hpp"
#include "Core/NodeGraph.hpp"

#include <algorithm>
#include <print>
#include <ranges>


namespace st::core
{
	class ContentManager;
} // namespace st::core
namespace st::ui
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

	class NodeNameBase : public QAbstractGraphicsShapeItem
	{
	  public:
		NodeNameBase(const QString& name, QGraphicsItem* parent = nullptr);

		QRectF boundingRect() const override;

		void paint(QPainter* painter,
				   const QStyleOptionGraphicsItem* option,
				   QWidget* widget) override;

	  private:
		QString m_name;
	};

	/**
	 * @brief Represent node plug in the scene
	 *
	 * MARK: NodePlug
	 */
	class NodePlug : public QAbstractGraphicsShapeItem
	{
	  private:
		static constexpr QColor nodeConnectionColor = QColor(85, 209, 208);
		static constexpr QColor nodeConnectionHover = QColor(175, 233, 233);

	  public:
		NodePlug(QGraphicsItem* parent = nullptr);

		QRectF boundingRect() const override;
		;

		void paint(QPainter* painter,
				   const QStyleOptionGraphicsItem* option,
				   QWidget* widget) override;

		void setConnected(bool state);

		bool isConnected() const;

		void beginConnection();

		void endConnection();

		NodeAttribute* getParentAttribute() const;

		QPointF getPlugCenterPosition() const;

	  private:
		bool m_isHovered = false;
		bool m_isConnected = false;
		bool m_isConnecting = false;

		QPainterPath createConnectionPath();
	};

	/**
	 * @brief Represent attribute in the node
	 *
	 * MARK: NodeAttribute
	 */
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
		NodeAttribute(std::shared_ptr<core::Attribute> attribute, QGraphicsItem* parent = nullptr);

		QRectF boundingRect() const override;

		void paint(QPainter* painter,
				   [[maybe_unused]] const QStyleOptionGraphicsItem* option,
				   [[maybe_unused]] QWidget* widget) override;

		/*--------------------------------*/
		/*---------Event Handlers---------*/
		/*--------------------------------*/
		void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

		/*--------------------------------*/
		/*---------Getter/Setters---------*/
		/*--------------------------------*/
		NodePlug* getInputPlug() const;
		NodePlug* getOutputPlug() const;
		NodeItem* getParentNode() const;

		std::shared_ptr<core::Attribute> getAttribute() const;

	  private:
		std::shared_ptr<core::Attribute> m_attribute; // Handler to Attribute
		NodePlug* m_pInputPlug;
		NodePlug* m_pOutputPlug;
	};

	/**
	 * @brief NodeItem represent node in the scene
	 *
	 * MARK: NodeItem
	 */
	class NodeItem : public QAbstractGraphicsShapeItem
	{
		static constexpr int32_t NodeWidth = 300;
		static constexpr int32_t NodeHeight = 400;

	  public:
		NodeItem(std::weak_ptr<core::Node> node, QGraphicsItem* parent = nullptr);

		virtual QRectF boundingRect() const override;

		virtual void paint(QPainter* painter,
						   const QStyleOptionGraphicsItem* option,
						   QWidget* widget) override;

		void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

		std::vector<NodeAttribute*> getAttributes() const;
		std::weak_ptr<core::Node> getNode() const;

	  private:
		std::weak_ptr<core::Node> m_node;
		std::vector<NodeAttribute*> m_attributes;
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

	/**
	 * @brief Represent connection between two nodes
	 *
	 * MARK: NodeConnection
	 */
	class NodeConnection : public QAbstractGraphicsShapeItem
	{
		static constexpr QColor nodeConnectionColor = QColor(85, 209, 208);

		// Position of the plug
		static constexpr QPointF plugPosition = QPointF(0.0f, 12.5f);

	  public:
		NodeConnection(std::weak_ptr<core::Connection> connection, QGraphicsItem* parent = nullptr);

		void initialize();

		QRectF boundingRect() const override;
		void paint(QPainter* painter,
				   const QStyleOptionGraphicsItem* option,
				   QWidget* widget) override;

	  private:
		std::weak_ptr<core::Connection> m_connection;
		QPointF m_startPos;
		QPointF m_endPos;
	};

	/**
	 * @brief Represent node graph in the scene
	 *
	 * MARK: NodeScene
	 */
	class NodeScene : public QGraphicsScene
	{
		Q_OBJECT

		constexpr static int32_t sceneWidth = 10000;
		constexpr static int32_t sceneHeight = 10000;

		enum class State
		{
			eUndefined,
			eIdle,
			eConnecting,
			eNrOfStates
		};

	  public:
		explicit NodeScene(QObject* parent = nullptr);

		void setNodeGraph(core::NodeGraph* nodeGraph);
		void updateScene();

		void addNode(std::weak_ptr<core::Node> node);
		void removeNode(std::weak_ptr<core::Node> node);

		void addConnection(std::weak_ptr<core::Connection> connection);
		void removeConnection(std::weak_ptr<core::Connection> connection);

		void drawBackground(QPainter* painter, const QRectF& rect) override;

		void onConnectionStart(NodePlug* plug)
		{
			m_state = State::eConnecting;
			plug->beginConnection();
			m_pTempConnection = new PlugConnection(plug, plug);
		}

		/*--------------------------------*/
		/*---------Event Handlers---------*/
		/*--------------------------------*/
		void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

	  private:
		core::NodeGraph* m_nodeGraph;
		std::unordered_map<std::weak_ptr<core::Node>, NodeItem*, WeakPtrHash, WeakPtrEqual> m_nodes;

		PlugConnection* m_pTempConnection = nullptr;
		std::vector<PlugConnection*> m_connections;

		QPointF currentLineStart;
		QPointF currentLineEnd;
		State m_state;
	};

	/**
	 * @brief View for the node editor
	 *
	 * MARK: NodeEditor
	 */

	class NodeEditor : public QGraphicsView
	{
		Q_OBJECT

	  public:
		explicit NodeEditor(core::ContentManagerHandler contentManager, QWidget* parent = nullptr);

		void initialize();

	  protected:
		/*--------------------------------*/
		/*---------Event Handlers---------*/
		/*--------------------------------*/
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

		/*------------------------*/
		/*------User Input--------*/
		/*------------------------*/
		bool m_panning;
		QPointF m_lastPanPoint;
	};
} // namespace st::ui

#endif // ST_UI_NODEEDITOR_HPP
