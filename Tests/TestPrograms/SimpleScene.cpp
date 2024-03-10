#include <iostream>
#include "Core/Scene.hpp"
#include "Core/Camera.hpp"
#include "Core/Transform.hpp"

using namespace st;


int main(int, char* [])
{
    std::cout << "Hello, World!" << std::endl;



    //Create Scene
    core::Scene scene;

    //Add Camera
    core::Camera camera{45.0f, 35.0f, 0.1f, 10000.0f};
    scene.addCamera(camera);

    core::Transform transform{camera};
    transform.setTranslation(Eigen::Vector3f{0.0f, 0.0f, 6.0f});

    //Add Grid

    //Add Axis

    //Add Sphere

    //Render Scene



    return 0;
}