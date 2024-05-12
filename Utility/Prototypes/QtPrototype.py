import sys
from PyQt6.QtWidgets import (QApplication, 
                             QGraphicsScene, 
                             QGraphicsView, 
                             QLabel, 
                             QMainWindow, 
                             QHBoxLayout, 
                             QWidget)

if __name__ == '__main__':
    app = QApplication(sys.argv)

    mainWindow = QMainWindow()
    mainWindow.setContentsMargins(10, 10, 10, 10)
    mainWindow.setMinimumSize(600,600)
    mainWidget = QWidget()
    mainWidget.setContentsMargins(0, 0, 0, 0)
    hBoxLayout = QHBoxLayout(mainWidget)
    hBoxLayout.setContentsMargins(0, 0, 0, 0)


    # Create a QGraphicsScene
    scene = QGraphicsScene()
    view = QGraphicsView(scene)
    view.setContentsMargins(0, 0, 0, 0)
    mainWindow.setCentralWidget(view)

    hBoxLayout.addWidget(view)

    # Show the view
    mainWindow.setCentralWidget(mainWidget)
    mainWindow.show()

    sys.exit(app.exec())