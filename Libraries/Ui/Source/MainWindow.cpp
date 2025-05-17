#include "MainWindow.hpp"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QPushButton>

#include "Core/ContentManager.hpp"
#include "NodeEditor.hpp"
#include "Viewport.hpp"
#include "AttributeEditor.hpp"
#include "Outliner.hpp"
#include "MaterialBrowser.hpp"

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

		m_materialBrowser = new MaterialBrowser(contentManager);
		m_materialBrowser->setContentsMargins(0, 0, 0, 0);
		m_materialBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		QTabWidget* tabWidget = new QTabWidget(this);
		tabWidget->setContentsMargins(0, 0, 0, 0);
		tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		tabWidget->setTabsClosable(false);
		tabWidget->setMovable(false);
		tabWidget->setTabPosition(QTabWidget::North);

		tabWidget->addTab(m_nodeEditor, "Node Editor");
		tabWidget->addTab(m_materialBrowser, "Material Browser");

		connect(tabWidget, &QTabWidget::currentChanged, this, [this, tabWidget](int index) {
			spdlog::info("Tab changed to index: {}", index);
			if (tabWidget->widget(index) == m_nodeEditor) {
				spdlog::info("Node Editor tab selected");
				// Force the node editor to refresh
				m_nodeEditor->refreshNodeGraph();
			}
		});


		m_attributeEditor = new AttributeEditor(contentManager);
		m_attributeEditor->setContentsMargins(0, 0, 0, 0);
		m_attributeEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		QWidget* leftPanel = new QWidget(this);
		QVBoxLayout* leftPanelLayout = new QVBoxLayout(leftPanel);
		leftPanelLayout->setContentsMargins(10, 10, 10, 10);
		leftPanelLayout->addWidget(new Outliner(contentManager));
		leftPanel->setLayout(leftPanelLayout);



		QWidget* centralPanel = new QWidget(this);
		QVBoxLayout* vBoxLayout = new QVBoxLayout();
		vBoxLayout->setContentsMargins(10, 10, 10, 10);
		vBoxLayout->addWidget(m_viewport);
		vBoxLayout->addWidget(tabWidget);
		vBoxLayout->setStretchFactor(m_viewport, 2);
		vBoxLayout->setStretchFactor(tabWidget, 1);
		centralPanel->setLayout(vBoxLayout);

		QWidget* rightPanel = new QWidget(this);
		QVBoxLayout* attributeLayout = new QVBoxLayout();
		attributeLayout->setContentsMargins(10, 10, 10, 10);
		attributeLayout->addWidget(new QLabel("Attribute Editor"));
		attributeLayout->addWidget(m_attributeEditor);
		attributeLayout->setStretchFactor(m_attributeEditor, 1);
		attributeLayout->addStretch(1);
		rightPanel->setLayout(attributeLayout);


		QSplitter* splitter = new QSplitter(Qt::Horizontal);
		splitter->addWidget(leftPanel);
		splitter->addWidget(centralPanel);
		splitter->addWidget(rightPanel);
		splitter->setStyleSheet("QSplitter::handle { background: #2E2E2E; }");

		splitter->setSizes(QList<int> { 200, 600, 200 });

		centralLayout->addWidget(splitter);

		setCentralWidget(centralWidget);

		setWindowState(Qt::WindowMaximized);
	}
} // namespace st::ui