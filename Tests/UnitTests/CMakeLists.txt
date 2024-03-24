include(CTest)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)




add_subdirectory(Geometry)
add_subdirectory(Renderer)