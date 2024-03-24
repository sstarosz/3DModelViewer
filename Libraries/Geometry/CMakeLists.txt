cmake_minimum_required(VERSION 3.26)

project(Geometry
		VERSION 0.0.1
		DESCRIPTION "Rendering Engine "
		LANGUAGES CXX)

        
find_package(Eigen3 CONFIG REQUIRED)

set(Sources
    "Source/Line.cpp"
    "Source/Point3d.cpp"
    "Source/Geometry.cpp"
    )

set(Public_Headers
    "Include/Geometry/Line.hpp"
    "Include/Geometry/Point3d.hpp"
    "Include/Geometry/Geometry.hpp"
    )

set(Private_Headers
    )

add_library(${PROJECT_NAME} ${Sources} ${Public_Headers} ${Private_Headers})

target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME})



target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} PUBLIC 
                        Eigen3::Eigen
                     )


#generate_documentation(TargetName)
