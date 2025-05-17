#include "MaterialBrowser.hpp"

#include <QHBoxLayout>
#include <QLabel>

namespace st::ui
{

    MaterialBrowser::MaterialBrowser(core::ContentManagerHandler contentManager, QWidget* parent)
    {
        auto& nodeGraph = contentManager->getMainNodeGraph();

        for(const auto& node : nodeGraph.getNodes())
        {
            
        }

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->setAlignment(Qt::AlignTop);
        layout->addWidget(new QLabel("Material Browser", this));
        layout->addWidget(new QLabel("Material Browser", this));
        layout->addWidget(new QLabel("Material Browser", this));

        setLayout(layout);
    }

}