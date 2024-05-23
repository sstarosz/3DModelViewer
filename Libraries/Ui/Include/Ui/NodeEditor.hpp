#ifndef ST_UI_NODEEDITOR_HPP
#define ST_UI_NODEEDITOR_HPP

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "Core/Nodes/Node.hpp"
#include "Core/NodeGraph.hpp"

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
				QGraphicsItem(parent)
			{
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
				qreal penWidth = 1;
				return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
			}

			void paint(QPainter* painter,
					   const QStyleOptionGraphicsItem* option,
					   QWidget* widget) override
			{
				// Draw black
				painter->setPen(QPen(Qt::black, 1));
				painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
			}

            private:
                std::weak_ptr<core::Node2> m_node;

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
            void resizeEvent(QResizeEvent* event) override;

		  private:
			void setupScene();
            void drawAxes();

			NodeScene* m_scene;
			core::ContentManager& m_contentManager;

            QPoint m_lastPanPoint;
            QGraphicsLineItem* m_yAxis;
            QGraphicsLineItem* m_xAxis;
		};

	} // namespace ui
} // namespace st

#endif // ST_UI_NODEEDITOR_HPP
