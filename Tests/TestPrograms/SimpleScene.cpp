#include "Core/Scene.hpp"
#include "Core/Camera.hpp"
#include "Core/Transform.hpp"

#include "Renderer/Renderer.hpp"
#include "Ui/Viewport.hpp"
#include "Renderer/Grid.hpp"
#include "Renderer/Plane.hpp"
#include "Renderer/StandardMaterial.hpp"
#include "Renderer/MaterialManager.hpp"
#include "Renderer/Mesh.hpp"

#include <QApplication>

using namespace st;


namespace st{
    class Application
    {
    public:
        Application(int argc, char* argv[])
        {
            QApplication a(argc, argv);
        }

        int exec()
        {
            return QApplication::exec();
        }
    };
    
}


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    ui::Viewport viewport;

    renderer::Renderer renderer;

    //Create Scene
    //std::shared_ptr<core::Scene> scene = std::make_shared<core::Scene>();

    //Add Camera
    //core::Camera camera{45.0f, 35.0f, 0.1f, 10000.0f};
    //core::Transform transform{camera};
    //transform.setTranslation(Eigen::Vector3f{0.0f, 0.0f, 6.0f});
    //scene->addCamera(std::move(camera));

    //Add Camera v2
    core::Camera camera;
    std::weak_ptr<core::StObject> cameraObject = camera.create();
    camera.setAngleOfView(45.0f);
    camera.setFocalLength(35.0f);
    camera.setNearClippingPlane(0.1f);
    camera.setFarClippingPlane(10000.0f);

    core::Transform transform{cameraObject};
    transform.setTranslation(Eigen::Vector3f{0.0f, 0.0f, 6.0f});

    /*-----------------------------------------------------------------------------------------------*/
    /*--------------------------------------Add Scene Content----------------------------------------*/
    /*-----------------------------------------------------------------------------------------------*/
    //Add Grid
    //TODO
    //renderer::Grid grid(10.0f, 1);
    //scene->addNode(std::move(grid));

    //Add Axis
    //TODO

    //Add Sphere
    //TODO

    //Add Plane
    renderer::MeshFactory meshFactory;
    renderer::Plane plane;
    std::weak_ptr<core::StObject> planeObject = meshFactory.create(plane);



    /*-----------------------------------------------------------------------------------------------*/
    /*--------------------------------------Add Material---------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------*/
    const renderer::MaterialManager& materialManager = renderer.getMaterialManager();
    
    renderer::StandardMaterial material;
    materialManager.createMaterial(material);

    //Assign Material to Plane
    materialManager.assignMaterialToMesh(material, planeObject);


    /*-----------------------------------------------------------------------------------------------*/
    /*-------------------------------------Setup renderer--------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------*/
    //embed renderer in viewport
    //renderer.embedScene(scene);
    

    //Show Viewport
    viewport.embedRenderer(std::move(renderer));

    viewport.setGeometry(100, 100, 800, 600);
    viewport.show();
    return QApplication::exec();
}