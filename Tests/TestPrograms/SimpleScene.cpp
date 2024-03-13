#include <iostream>
#include "Core/Scene.hpp"
#include "Core/Camera.hpp"
#include "Core/Transform.hpp"

#include "Renderer/Renderer.hpp"
#include "Ui/Viewport.hpp"

#include <QApplication>



using namespace st;





int main(int argc, char* argv[])
{
    std::cout << "Hello, World!" << std::endl;
    QApplication a(argc, argv);

    ui::Viewport viewport;

    renderer::Renderer renderer;

    //Create Scene
    core::Scene scene;

    //Add Camera
    core::Camera camera{45.0f, 35.0f, 0.1f, 10000.0f};
    core::Transform transform{camera};
    transform.setTranslation(Eigen::Vector3f{0.0f, 0.0f, 6.0f});
    scene.addCamera(std::move(camera));

    //Add Grid

    //Add Axis

    //Add Sphere

    //Render Scene

    //embed scene in renderer
    //embed renderer in viewport


    //viewport.embedRenderer(&renderer);

    
    viewport.show();
    return QApplication::exec();
}