#ifndef ST_UI_OUTLINER_HPP
#define ST_UI_OUTLINER_HPP


#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSplitter>
#include <QVBoxLayout>

#include "Core/ContentManager.hpp"
#include "Core/Node.hpp"

namespace st::ui
{
    class Outliner : public QWidget
    {
        Q_OBJECT

      public:
        explicit Outliner(core::ContentManagerHandler contentManager, QWidget* parent = nullptr);

        void showEvent(QShowEvent* event) override;
      private:
        core::ContentManagerHandler m_contentManager;
        QTreeWidget* m_treeWidget;
        QSplitter* m_splitter;
        QVBoxLayout* m_layout;

        void populateTree(const std::shared_ptr<core::Node>& node, QTreeWidgetItem* parentItem = nullptr);

      private slots:
        void onItemClicked(QTreeWidgetItem* item, int column);
        
    };

}

#endif // ST_UI_OUTLINER_HPP