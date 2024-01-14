#ifndef ST_UI_VIEWPORT_HPP
#define ST_UI_VIEWPORT_HPP

#include <QWidget>

namespace st::ui
{

class Viewport : public QWidget
{
    Q_OBJECT

    public:
        explicit Viewport(QWidget *parent = nullptr, Qt::WindowFlags flags = {});

    private:
        class PrivateWindow;
        PrivateWindow* m_window;
};

}//namespace st::ui

#endif //ST_UI_VIEWPORT_HPP
