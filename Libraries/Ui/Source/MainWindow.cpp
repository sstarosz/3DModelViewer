#include "MainWindow.hpp"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>

#include "Core/ContentManager.hpp"
#include "NodeEditor.hpp"
#include "Viewport.hpp"
#include "AttributeEditor.hpp"

#include <spdlog/spdlog.h>

namespace st::ui
{
	MainWindow::MainWindow(st::core::ContentManagerHandler contentManager,
						   QWidget* parent,
						   Qt::WindowFlags flags) :
		QMainWindow(parent, flags),
		m_contentManager(contentManager)
	{
		spdlog::info("MainWindow::MainWindow()");

		setWindowTitle("StModelViewer");
		setContentsMargins(0, 0, 0, 0);

		QWidget* centralWidget = new QWidget(this);
		QHBoxLayout* centralLayout = new QHBoxLayout(centralWidget);
		centralLayout->setContentsMargins(0, 0, 0, 0);

		m_viewport = new Viewport(contentManager);
		m_viewport->setContentsMargins(0, 0, 0, 0);
		m_viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		m_nodeEditor = new NodeEditor(contentManager);
		m_nodeEditor->setContentsMargins(0, 0, 0, 0);
		m_nodeEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		m_attributeEditor = new AttributeEditor(contentManager);
		m_attributeEditor->setContentsMargins(0, 0, 0, 0);
		m_attributeEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		QVBoxLayout* vBoxLayout = new QVBoxLayout();
		vBoxLayout->setContentsMargins(10, 10, 10, 10);
		vBoxLayout->addWidget(m_viewport);
		vBoxLayout->addWidget(m_nodeEditor);
		vBoxLayout->setStretchFactor(m_viewport, 2);
		vBoxLayout->setStretchFactor(m_nodeEditor, 1);

		QVBoxLayout* attributeLayout = new QVBoxLayout();
		attributeLayout->setContentsMargins(10, 10, 10, 10);
		attributeLayout->addWidget(m_attributeEditor);
		attributeLayout->setStretchFactor(m_attributeEditor, 1);
		attributeLayout->addStretch(1);
		attributeLayout->addWidget(new QLabel("Attribute Editor"));


		centralLayout->addLayout(vBoxLayout);
		centralLayout->addLayout(attributeLayout);


		setCentralWidget(centralWidget);
		setWindowState(Qt::WindowMaximized);
	}

	void MainWindow::initialize()
	{
		spdlog::info("MainWindow::initialize()");
		m_viewport->initialize();
		m_nodeEditor->initialize();
	}

} // namespace st::ui