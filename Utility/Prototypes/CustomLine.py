from PyQt6.QtWidgets import QGraphicsItem, QGraphicsScene, QGraphicsView, QApplication
from PyQt6.QtGui import QPainter, QLinearGradient, QBrush, QPen, QPainterPath
from PyQt6.QtCore import QPointF, QRectF, Qt, QLineF

class GradientLineItem(QGraphicsItem):
    def __init__(self, start, end, start_thickness, end_thickness, color=Qt.GlobalColor.black):
        super().__init__()
        self.start = start
        self.end = end
        self.start_thickness = start_thickness
        self.end_thickness = end_thickness
        self.color = color

    def boundingRect(self):
        return QRectF(self.start, self.end).normalized()

    def paint(self, painter, option, widget):
        gradient = QLinearGradient(self.start, self.end)
        gradient.setColorAt(0, self.color)
        gradient.setColorAt(1, self.color)

        painter.setPen(Qt.PenStyle.NoPen)
        painter.setBrush(QBrush(gradient))
        painter.setRenderHint(QPainter.RenderHint.Antialiasing)

        path = QPainterPath()
        line = QLineF(self.start, self.end)
        normal = line.normalVector().unitVector()

        # Adjust normal length for rounded start and end
        normal.setLength(self.start_thickness / 2.0)
        p1 = self.start + normal.p2()
        p2 = self.start - normal.p2()

        normal.setLength(self.end_thickness / 2.0)
        p3 = self.end - normal.p2()
        p4 = self.end + normal.p2()

        # Create the path with rounded start and end
        path.moveTo(p1)
        path.lineTo(p2)
        path.quadTo(self.start, p2)  # Rounded start
        path.lineTo(p3)
        path.quadTo(self.end, p3)  # Rounded end
        path.lineTo(p4)
        path.closeSubpath()

        painter.drawPath(path)

if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)

    scene = QGraphicsScene()
    view = QGraphicsView(scene)

    line = GradientLineItem(QPointF(50, 50), QPointF(250, 250), 10, 2, Qt.GlobalColor.blue)
    scene.addItem(line)

    view.show()
    sys.exit(app.exec())
