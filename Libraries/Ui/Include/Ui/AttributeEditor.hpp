#ifndef ST_UI_ATTRIBUTEEDITOR_HPP
#define ST_UI_ATTRIBUTEEDITOR_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
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

      private:
        void initialize();

        core::ContentManagerHandler m_contentManager;
    };
} // namespace st::ui



#endif // ST_UI_ATTRIBUTEEDITOR_HPP