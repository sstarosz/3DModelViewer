#include "AttributeEditor.hpp"
#include <QVBoxLayout>

namespace st::ui
{

AttributeEditor::AttributeEditor(st::core::ContentManagerHandler contentManager, QWidget* parent):
    QWidget(parent),
    m_contentManager(contentManager)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(new QLabel("Attribute Editor", this));
}

void AttributeEditor::initialize()
{

}

} // namespace st::ui