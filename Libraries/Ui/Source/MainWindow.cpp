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
		m_viewport->setContentsMargins(0, 0, 0, 0);
		m_viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		m_nodeEditor = new NodeEditor(contentManager);
		m_nodeEditor->setContentsMargins(0, 0, 0, 0);
		m_nodeEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		QVBoxLayout* vBoxLayout = new QVBoxLayout();
		vBoxLayout->setContentsMargins(10, 10, 10, 10);
		vBoxLayout->addWidget(m_viewport);
		vBoxLayout->addWidget(m_nodeEditor);
		vBoxLayout->setStretchFactor(m_viewport, 2);
		vBoxLayout->setStretchFactor(m_nodeEditor, 1);

		centralLayout->addLayout(vBoxLayout);
		centralLayout->addWidget(new QLabel("Hello World!"));

		setCentralWidget(centralWidget);
		showMaximized();
	}

	void MainWindow::initialize()
	{
		m_nodeEditor->initialize();
	}

} // namespace st::ui