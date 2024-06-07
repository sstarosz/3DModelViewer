import sys
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QGraphicsView, QGraphicsScene, QSizePolicy

#Vulkan Window Widget
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QWindow, QSurface

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
        self.node_editor.setScene(QGraphicsScene())
        self.node_editor.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        main_layout.addWidget(self.node_editor)

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
