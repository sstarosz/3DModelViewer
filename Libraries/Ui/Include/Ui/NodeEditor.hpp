#ifndef ST_UI_NODEEDITOR_HPP
#define ST_UI_NODEEDITOR_HPP

#include <QGraphicsView>
#include <QGraphicsItem>

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
    NodeItem(std::weak_ptr<core::Node> node, QGraphicsItem* parent = nullptr):
        QGraphicsItem(parent)
    {

    }

    QRectF boundingRect() const override
    {
        qreal penWidth = 1;
        return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                      20 + penWidth, 20 + penWidth);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override
    {
        //Draw black
        painter->setPen(QPen(Qt::black, 1));
        painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
    }

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

private:
    void setupScene();

    QGraphicsScene *m_scene;
    core::ContentManager& m_contentManager;
};


} // namespace ui
} // namespace st

#endif // ST_UI_NODEEDITOR_HPP
