#ifndef ST_UI_NODEEDITOR_HPP
#define ST_UI_NODEEDITOR_HPP

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "Core/Nodes/Node.hpp"
#include "Core/NodeGraph.hpp"

#include <print>

namespace st
{

namespace core
{
	class ContentManager;
} // namespace core

namespace ui
{
	//#373B3E
	constexpr QColor NodeColor = QColor(55, 59, 62);
	//#5A676B
	constexpr QColor NodeBorderColor = QColor(90, 103, 107);
	//#55D1D0
	constexpr QColor NodeInputBarBorderColor = QColor(85, 209, 208);


	class NodeNameBase : public QGraphicsItem
	{
		public:
		NodeNameBase(const QString& name, QGraphicsItem* parent = nullptr) : QGraphicsItem(parent),
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

	class NodeInput : public QGraphicsItem
	{
	  public:
		NodeInput(const core::InputHandler& inputHandler,
				  QGraphicsItem* parent = nullptr) :
			QGraphicsItem(parent),
			m_inputHandler(inputHandler)
		{
		}

		QRectF boundingRect() const override
		{
			return QRectF(0, 0, 280, 25);
		}

		void paint(QPainter* painter,
					const QStyleOptionGraphicsItem* option,
					QWidget* widget) override
		{
			// Draw black
			painter->setPen(QPen(NodeInputBarBorderColor, 2));
			painter->setBrush(NodeBorderColor);
			painter->drawRoundedRect(0, 0, 280, 25, 5, 5);

			// Draw text
			painter->setPen(Qt::white);
			painter->setFont(QFont("Inter", 14));

			QRectF textRect(10, 0, 260, 25);
			painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(m_inputHandler.getName()));
		}

		private:
		core::InputHandler m_inputHandler;
	};

	class NodeItem : public QGraphicsItem
	{
		public:
		NodeItem(std::weak_ptr<core::Node2> node, 
				QGraphicsItem* parent = nullptr) :
			QGraphicsItem(parent),
			m_node(node),
			m_isHovered(false)
		{
			setAcceptHoverEvents(true);

			if(auto nodePtr = m_node.lock())
			{
				m_nodeName = QString::fromStdString(nodePtr->getName());
				std::println("Node: {}", nodePtr->getName());
				NodeNameBase* NodeName = new NodeNameBase(m_nodeName, this);	

				auto inputHandlers = nodePtr->getInputHandlers();
				uint32_t inputYOffset = 55;
				uint32_t inputXOffset = 10;
				for(auto& inputHandler : inputHandlers)
				{
					//auto& input = inputHandler.getInput();
					std::println("Input: {}", inputHandler.getName());

					NodeInput* input = new NodeInput(inputHandler, this);
					input->setPos(inputXOffset, inputYOffset);
					inputYOffset += 29;
				}
			}

		}

		void setPosition(const QPointF& position)
		{
			setPos(position);
		}

		QPointF getPosition() const
		{
			return pos();
		}

		QRectF boundingRect() const override
		{
			return QRectF(0, 0, 300, 400);
		}

		void paint(QPainter* painter,
					const QStyleOptionGraphicsItem* option,
					QWidget* widget) override
		{
			// Draw black
			painter->setPen(QPen(NodeBorderColor, 3));
			painter->setBrush(NodeColor);
			painter->drawRoundedRect(0, 0, 300, 400, 20, 20);


			if(m_isHovered)
			{
				painter->setPen(QPen(Qt::red, 1));
				painter->setBrush(Qt::transparent);
				painter->drawRoundedRect(-10, -10, 100, 100, 5, 5);
			}
		}


		void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override
		{
			std::println("Hovered");
			m_isHovered = true;
			update();
			QGraphicsItem::hoverEnterEvent(event);
		}

		void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override
		{
			std::println("Not Hovered");
			m_isHovered = false;
			update();
			QGraphicsItem::hoverLeaveEvent(event);
		}


		private:
			std::weak_ptr<core::Node2> m_node;
			bool m_isHovered;
			QPointF m_position;
			QString m_nodeName;

			static constexpr int32_t NnodeWidth = 100;
			static constexpr int32_t NnodeHeight = 100;

	};

	class NodeScene : public QGraphicsScene
	{
		Q_OBJECT

		public:
			explicit NodeScene(QObject* parent = nullptr);

			void addNode(std::weak_ptr<core::Node2> node)
			{
				//auto nodeItem = new NodeItem(node);
				//addItem(nodeItem);
			}

			void removeNode(std::weak_ptr<core::Node2> node)
			{

			}

			void drawBackground(QPainter* painter, const QRectF& rect) override;

			void setView(QGraphicsView* view)
			{
				m_view = view;
			}

		private:
		QGraphicsView* m_view;
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
