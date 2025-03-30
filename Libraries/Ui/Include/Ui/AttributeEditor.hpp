#ifndef ST_UI_ATTRIBUTEEDITOR_HPP
#define ST_UI_ATTRIBUTEEDITOR_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

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
        explicit AttributeEditor(QWidget* parent = nullptr);

      private:
        void setupUi();
    };
} // namespace st::ui



#endif // ST_UI_ATTRIBUTEEDITOR_HPP