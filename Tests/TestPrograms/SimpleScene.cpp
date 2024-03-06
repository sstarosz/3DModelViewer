#include <iostream>
#include "Core/Scene.hpp"
#include "Core/Camera.hpp"

using namespace st;


int main(int, char* [])
{
    std::cout << "Hello, World!" << std::endl;



    //Create Scene
    core::Scene scene;

    //Add Camera
    core::Camera camera;
    scene.addCamera(camera);

    //Add Grid

    //Add Axis

    //Add Sphere

    //Render Scene



    return 0;
}