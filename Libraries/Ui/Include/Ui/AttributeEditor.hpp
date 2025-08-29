#ifndef ST_UI_ATTRIBUTEEDITOR_HPP
#define ST_UI_ATTRIBUTEEDITOR_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include "Core/ContentManager.hpp"


namespace st::ui
{
    /**
     * @brief Attribute editor for the node editor
     *
     * MARK: AttributeEditor
     */
    class AttributeEditor : public QWidget
    {
        Q_OBJECT

      public:
        explicit AttributeEditor(core::ContentManagerHandler contentManager, 
                                 QWidget* parent = nullptr);
        void showEvent(QShowEvent* event) override;
      private:
        void onSelectionChanged();
        void clearControls();
        QWidget* createControlAttributeWidget(const std::shared_ptr<core::Attribute>& attribute);

        core::ContentManagerHandler m_contentManager;
        QScrollArea* m_scrollArea;
        QWidget* m_scrollAreaWidgetContents;
    };
} // namespace st::ui



#endif // ST_UI_ATTRIBUTEEDITOR_HPP