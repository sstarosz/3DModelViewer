#include "MaterialBrowser.hpp"

#include <QHBoxLayout>
#include <QLabel>

namespace st::ui
{

    MaterialBrowser::MaterialBrowser(core::ContentManagerHandler contentManager, QWidget* parent):
        QWidget(parent),
        m_contentManager(contentManager)
    {
        setObjectName("MaterialBrowser");
        setMinimumWidth(200);
        setMinimumHeight(100);
    }
    

    void MaterialBrowser::showEvent(QShowEvent* event)
    {
        QWidget::showEvent(event);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->setAlignment(Qt::AlignTop);


        auto& nodeGraph = m_contentManager->getMainNodeGraph();
        for(const auto& node : nodeGraph.getNodes())
        {
            spdlog::info("MaterialBrowser::MaterialBrowser() - Node: {}", node->getName());
            if(node->getType() == core::Node::NodeType::eMaterial)
            {
                QLabel * label = new QLabel(node->getName().c_str(), this);
                label->setObjectName(node->getName().c_str());
                label->setStyleSheet("background-color: gray; color: white;");
                label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                label->setMinimumHeight(100);
                label->setMaximumHeight(100);
                label->setMinimumWidth(100);
                label->setMaximumWidth(100);
                label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

                layout->addWidget(label);
            }
        }

        setLayout(layout);
    }

}