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
	}

	namespace ui
	{

		class NodeItem : public QGraphicsItem
		{
		  public:
			NodeItem(std::weak_ptr<core::Node2> node, QGraphicsItem* parent = nullptr) :
				QGraphicsItem(parent),
				m_node(node),
				m_isHovered(false)
			{
				setAcceptHoverEvents(true);

				if(auto nodePtr = m_node.lock())
				{
					m_nodeName = QString::fromStdString(nodePtr->getName());
					std::print("{}", nodePtr->getName());
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
				return QRectF(-10, -10, 100, 100);
			}

			void paint(QPainter* painter,
					   const QStyleOptionGraphicsItem* option,
					   QWidget* widget) override
			{
				// Draw black
				painter->setPen(QPen(Qt::black, 1));
				painter->setBrush(Qt::black);
				painter->drawRoundedRect(-10, -10, 100, 100, 5, 5);

				painter->setPen(QPen(Qt::blue, 1));
				painter->setBrush(Qt::blue);
				painter->drawRoundedRect(-10, -10, 100, 20, 5, 5);
				painter->drawRect(-10, 0, 100, 10);

				// Draw text
				painter->setPen(Qt::white);
				painter->drawText(QRectF(-10, -10, 100, 20), Qt::AlignCenter, m_nodeName);


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
