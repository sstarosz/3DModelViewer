#include "MainWindow.hpp"
#include <QDebug>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>

#include "Viewport.hpp"
#include "NodeEditor.hpp"
#include "Core/ContentManager.hpp"

namespace st::ui
{
	MainWindow::MainWindow(st::core::ContentManager& contentManager, QWidget* parent, Qt::WindowFlags flags):
		QMainWindow(parent, flags),
		m_contentManager(contentManager)
	{
		setWindowTitle("StModelViewer");
		setContentsMargins(0, 0, 0, 0);

		QWidget* centralWidget = new QWidget(this);
		QHBoxLayout* centralLayout = new QHBoxLayout(centralWidget);
		centralLayout->setContentsMargins(0, 0, 0, 0);

		m_viewport = new Viewport();
		m_nodeEditor = new NodeEditor(contentManager);

		QVBoxLayout* vBoxLayout = new QVBoxLayout();
		vBoxLayout->setContentsMargins(10, 10, 10, 10);
		vBoxLayout->addWidget(m_viewport);
		vBoxLayout->addWidget(m_nodeEditor);

		centralLayout->addLayout(vBoxLayout);
		centralLayout->addWidget(new QLabel("Hello World!"));

		setCentralWidget(centralWidget);
	}

	void MainWindow::initialize()
	{
		m_nodeEditor->initialize();
	}

} // namespace st::ui