#ifndef ST_UI_MATRIX4WIDGET_HPP
#define ST_UI_MATRIX4WIDGET_HPP

#include <QWidget>
#include <Eigen/Dense>

namespace Ui
{
    class Matrix4Widget : public QWidget
    {
        Q_OBJECT

      public:
        explicit Matrix4Widget(QWidget* parent = nullptr);

        void setMatrix(const Eigen::Matrix4f& matrix);
        Eigen::Matrix4f getMatrix() const;

      signals:
        void matrixChanged(const Eigen::Matrix4f& matrix);

      private:
        Eigen::Matrix4f m_matrix;
    };
} // namespace Ui