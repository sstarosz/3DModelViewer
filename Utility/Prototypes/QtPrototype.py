import sys
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QGraphicsView, QGraphicsScene, QSizePolicy

#Vulkan Window Widget
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QWindow, QSurface
from PyQt6.QtWidgets import QAbstractGraphicsShapeItem
from PyQt6.QtGui import QPainter, QBrush
from PyQt6.QtCore import QRectF


class CustomEllipseItem(QAbstractGraphicsShapeItem):
    def __init__(self, x, y, width, height):
        super().__init__()
        self.rect = QRectF(x, y, width, height)
        self.setBrush(QBrush(Qt.BrushStyle.SolidPattern))
        self.setAcceptHoverEvents(True)

    def boundingRect(self):
        return self.rect

    def paint(self, painter: QPainter, option, widget=None):
        painter.setBrush(self.brush())
        painter.drawEllipse(self.rect)

    def hoverEnterEvent(self, event):
        self.setBrush(QBrush(Qt.GlobalColor.red))
        self.update()

    def hoverLeaveEvent(self, event):
        self.setBrush(QBrush(Qt.GlobalColor.green))
        self.update()


class VulkanWindow(QWindow):
    def __init__(self):
        super().__init__()
        self.setSurfaceType(QSurface.SurfaceType.OpenGLSurface)
        self.create()

class VulkanWidget(QWidget):

    def __init__(self) -> None:
        super().__init__()
        self.vulkan_window = VulkanWindow()

        window = QWidget.createWindowContainer(self.vulkan_window, self)

        layout = QVBoxLayout()
        layout.addWidget(window)
        self.setLayout(layout)
    



class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Node Editor with Viewport")
        self.setGeometry(0, 0, 400, 400)  # Initial window size

        # Central widget to hold the main layout
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        # Main layout - vertical layout
        main_layout = QVBoxLayout(central_widget)

        # Node Editor
        self.node_editor = QGraphicsView()
        self.scene = QGraphicsScene()
        self.node_editor.setScene(self.scene)
        self.node_editor.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        main_layout.addWidget(self.node_editor)


        # Add custom ellipse item to the scene
        ellipse_item = CustomEllipseItem(0, 0, 100, 50)
        self.scene.addItem(ellipse_item)

        # Viewport
        self.viewport = VulkanWidget()
        self.viewport.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        self.viewport.setStyleSheet("background-color: lightgray;")
        main_layout.addWidget(self.viewport)

        # Set layout margins and spacing
        #main_layout.setContentsMargins(0, 0, 0, 0)
        #main_layout.setSpacing(0)

        self.showMaximized()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
