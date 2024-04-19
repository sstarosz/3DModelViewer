find_package(GTest CONFIG REQUIRED)
include(GoogleTest)



add_subdirectory(Core)
add_subdirectory(Geometry)

if(Build_Tests_For_Gpu)
    add_subdirectory(Renderer)
endif(Build_Tests_For_Gpu)
