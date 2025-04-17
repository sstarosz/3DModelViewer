#include "Outliner.hpp"

#include "Core/EventRegistry.hpp"

namespace st::ui
{
    Outliner::Outliner(core::ContentManagerHandler contentManager, QWidget* parent)
    : QWidget(parent),
      m_contentManager(contentManager),
      m_treeWidget(new QTreeWidget(this)),
      m_layout(new QVBoxLayout(this))
    {
        setWindowTitle("Outliner");

        m_treeWidget->setHeaderLabel("Nodes");
        m_treeWidget->setColumnCount(1);
        m_treeWidget->setColumnWidth(0, 200);

        m_layout->addWidget(m_treeWidget);
        setLayout(m_layout);

        connect(m_treeWidget, &QTreeWidget::itemClicked, this, &Outliner::onItemClicked);
    }

	void Outliner::showEvent([[maybe_unused]] QShowEvent* event)
	{
        // Populate the tree with nodes from the content manager
        for (const auto& node : m_contentManager->getMainNodeGraph().getNodes())
        {
            populateTree(node);
        }
	}

	void Outliner::populateTree(const std::shared_ptr<core::Node>& node, QTreeWidgetItem* parentItem)
	{
        if (!node)
        {
            return; // Skip if node is null
        }
    
        QTreeWidgetItem* item;
        
        if (parentItem)
        {
            // Create as a child item of the parent
            item = new QTreeWidgetItem(parentItem);
        }
        else
        {
            // Create without parent, then add as top-level item
            item = new QTreeWidgetItem();
            m_treeWidget->addTopLevelItem(item);
        }
        
        item->setText(0, QString::fromStdString(node->getName()));
        item->setData(0, Qt::UserRole, QVariant::fromValue(node)); // Store the node pointer in the item
        
        // Recursively add child nodes
        for (const auto& childNode : node->getChildNodes())
        {
            populateTree(childNode, item);
        }

        //Add attributes of the node to the item
        for (const auto& attribute : node->getAttributes())
        {
            QTreeWidgetItem* attrItem = new QTreeWidgetItem(item);
            attrItem->setText(0, QString::fromStdString(attribute->getName()));
            attrItem->setData(0, Qt::UserRole, QVariant::fromValue(attribute)); // Store the attribute pointer in the item
        }
	}

    void Outliner::onItemClicked(QTreeWidgetItem* item, int column)
    {
        if (!item) return;
        
        // Get the stored data
        QVariant itemData = item->data(0, Qt::UserRole);
        
        // Check if this is a node item
        if (itemData.canConvert<std::shared_ptr<core::Node>>()) {
            auto node = itemData.value<std::shared_ptr<core::Node>>();
            if (node) {
                // Update selection in ContentManager
                m_contentManager->updateSelection(node);
                core::EventRegistry::sendEvent(core::CoreEvents::SelectionChanged);
                qDebug() << "Selected node:" << QString::fromStdString(node->getName());
            }
        }
        // Check if this is an attribute item
        else if (itemData.canConvert<std::shared_ptr<core::Attribute>>()) {
            auto attribute = itemData.value<std::shared_ptr<core::Attribute>>();
            if (attribute) {
                qDebug() << "Selected attribute:" << QString::fromStdString(attribute->getName());
                // Handle attribute selection as needed
            }
        }
    }
}

