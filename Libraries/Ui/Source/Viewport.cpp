#include "Viewport.hpp"
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QHBoxLayout>
#include <QDebug>

namespace st::ui
{
    Viewport::Viewport(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags)
    {
        QVulkanInstance *vulkanInstance = new QVulkanInstance();
        vulkanInstance->setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");
        vulkanInstance->setExtensions(QByteArrayList() << "VK_KHR_surface" << "VK_KHR_win32_surface");

        if (!vulkanInstance->create())
        {
            qDebug() << "Failed to create Vulkan instance:" << vulkanInstance->errorCode();
        }
        


        QVulkanWindow *vulkanWindow = new QVulkanWindow();
        vulkanWindow->setVulkanInstance(vulkanInstance);

        QWidget *vulkaWidget = QWidget::createWindowContainer(vulkanWindow, this);

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(vulkaWidget);
    }

}//namespace st::ui