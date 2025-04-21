#include "AttributeEditor.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include "Core/EventRegistry.hpp"
#include "Core/Node.hpp"
#include "Core/Attribute.hpp"


namespace st::ui
{

AttributeEditor::AttributeEditor(st::core::ContentManagerHandler contentManager, QWidget* parent):
    QWidget(parent),
    m_contentManager(contentManager)
{
    
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_scrollAreaWidgetContents = new QWidget();
    m_scrollAreaWidgetContents->setGeometry(QRect(0, 0, 300, 500));
    m_scrollArea->setWidget(m_scrollAreaWidgetContents);

    auto layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(new QLabel("Node: None", this));
    m_scrollAreaWidgetContents->setLayout(layout);
    
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_scrollArea);
    mainLayout->setAlignment(Qt::AlignTop);

    setLayout(mainLayout);
}

void AttributeEditor::initialize()
{
    core::EventRegistry::subscribeToEvent(core::CoreEvents::SelectionChanged,
        [this]() {
            onSelectionChanged();
        });
}

void AttributeEditor::onSelectionChanged()
{
    clearControls();

    QVBoxLayout* layout = static_cast<QVBoxLayout*>(m_scrollAreaWidgetContents->layout());

    //Experimental Selection
    layout->addWidget(new QLabel("Experimental Selection", this));
    layout->addWidget(new QLabel("Selection: ", this));
    for (const auto& selection : m_contentManager->getSelection().m_paths)
    {
        if(selection.isNode())
        {
            //TODO add node widget
            auto node = m_contentManager->getMainNodeGraph().getNodeByPath(selection);
            if(auto nodePtr = node.lock())
            {
                layout->addWidget(new QLabel(QString::fromStdString(nodePtr->getName()), this));
                for (const auto& attribute : nodePtr->getAttributes())
                {
                    layout->addWidget(new QLabel(QString::fromStdString(attribute->getName()), this));
                    if (auto controlWidget = createControlAttributeWidget(attribute))
                    {
                        layout->addWidget(controlWidget);
                    }
                    else
                    {
                        layout->addWidget(new QLabel("No control available", this));
                    }
                }
            }
            else
            {
                layout->addWidget(new QLabel("Node not found", this));
            }
        }
        else if(selection.isAttribute())
        {
            //TODO add attribute widget

        }
    }


}

void AttributeEditor::clearControls()
{
    auto layout = static_cast<QVBoxLayout*>(m_scrollAreaWidgetContents->layout());
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
}

QWidget* AttributeEditor::createControlAttributeWidget(const std::shared_ptr<core::Attribute>& attribute)
{
    if(!attribute->isWritable())
    {
        return nullptr;
    }
    //TODO Readonly attributes

    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);

    QLabel* nameLabel = new QLabel(QString::fromStdString(attribute->getName()), container);
    layout->addWidget(nameLabel);

    if(auto numericFloat = std::dynamic_pointer_cast<core::NumericAttribute<float>>(attribute))
    {
        QDoubleSpinBox* spinBox = new QDoubleSpinBox(container);
        spinBox->setRange(numericFloat->getLowerLimit(), numericFloat->getUpperLimit());
        spinBox->setSingleStep(0.1);
        spinBox->setValue(static_cast<double>(*numericFloat->getData()));

        connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [numericFloat](double value) {
            numericFloat->setData(static_cast<float>(value));
        });

        layout->addWidget(spinBox);
        return container;
    }
    else if(auto numericInt = std::dynamic_pointer_cast<core::NumericAttribute<int32_t>>(attribute))
    {
        QSpinBox* spinBox = new QSpinBox(container);
        spinBox->setRange(numericInt->getLowerLimit(), numericInt->getUpperLimit());
        spinBox->setSingleStep(1);
        spinBox->setValue(*numericInt->getData());

        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [numericInt](int value) {
            numericInt->setData(value);
        });

        layout->addWidget(spinBox);
        return container;
    }
    else if(auto numericUInt = std::dynamic_pointer_cast<core::NumericAttribute<uint32_t>>(attribute))
    {
        QSpinBox* spinBox = new QSpinBox(container);
        spinBox->setRange(numericUInt->getLowerLimit(), numericUInt->getUpperLimit());
        spinBox->setSingleStep(1);
        spinBox->setValue(*numericUInt->getData());

        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [numericUInt](int value) {
            numericUInt->setData(static_cast<uint32_t>(value));
        });

        layout->addWidget(spinBox);
        return container;
    }
    else if(auto vector3F = std::dynamic_pointer_cast<core::TypedAttribute<Eigen::Vector3f>>(attribute))
    {
        QDoubleSpinBox* xSpinBox = new QDoubleSpinBox(container);
        xSpinBox->setRange(-100000.0, 100000.0);
        xSpinBox->setSingleStep(0.1);
        xSpinBox->setValue(vector3F->getData()->x());

        QDoubleSpinBox* ySpinBox = new QDoubleSpinBox(container);
        ySpinBox->setRange(-100000.0, 100000.0);
        ySpinBox->setSingleStep(0.1);
        ySpinBox->setValue(vector3F->getData()->y());

        QDoubleSpinBox* zSpinBox = new QDoubleSpinBox(container);
        zSpinBox->setRange(-100000.0, 100000.0);
        zSpinBox->setSingleStep(0.1);
        zSpinBox->setValue(vector3F->getData()->z());

        connect(xSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [vector3F](double value) {
            vector3F->setData(Eigen::Vector3f(static_cast<float>(value), vector3F->getData()->y(), vector3F->getData()->z()));
        });

        connect(ySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [vector3F](double value) {
            vector3F->setData(Eigen::Vector3f(vector3F->getData()->x(), static_cast<float>(value), vector3F->getData()->z()));
        });

        connect(zSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [vector3F](double value) {
            vector3F->setData(Eigen::Vector3f(vector3F->getData()->x(), vector3F->getData()->y(), static_cast<float>(value)));
        });

        core::EventRegistry::addAttributeChangedCallback(attribute->getPath(), [this, xSpinBox, ySpinBox, zSpinBox](core::AttributeMessage msg, core::Path path) {

            if (msg == core::AttributeMessage::eAttributeChanged)
            {
                auto attribute = m_contentManager->getMainNodeGraph().getAttributeByPath(path);

                if (auto attributePtr = attribute.lock())
                {
                    auto vector3F = std::dynamic_pointer_cast<core::TypedAttribute<Eigen::Vector3f>>(attributePtr);
                    if (vector3F)
                    {
                        xSpinBox->setValue(vector3F->getData()->x());
                        ySpinBox->setValue(vector3F->getData()->y());
                        zSpinBox->setValue(vector3F->getData()->z());
                    }
                }
            }
        });

        
        layout->addWidget(xSpinBox);
        layout->addWidget(ySpinBox);
        layout->addWidget(zSpinBox);

        return container;
    }


	return nullptr;
}

QWidget* createControlAttributeWidget(core::Path path)
{
    //TODO create control widget for attribute
    return nullptr;
}

} // namespace st::ui
