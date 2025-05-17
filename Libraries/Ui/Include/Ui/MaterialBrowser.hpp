#ifndef ST_UI_MATERIALBROWSER_HPP
#define ST_UI_MATERIALBROWSER_HPP

#include "Core/ContentManager.hpp"

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>



namespace st::ui
{
    class MaterialBrowser : public QWidget
    {
        Q_OBJECT

      public:
        explicit MaterialBrowser(core::ContentManagerHandler contentManager, QWidget* parent = nullptr);

      private:
        core::ContentManagerHandler m_contentManager;
        QVBoxLayout* m_layout;

    };
} // namespace st::ui

#endif // ST_UI_MATERIALBROWSER_HPP

