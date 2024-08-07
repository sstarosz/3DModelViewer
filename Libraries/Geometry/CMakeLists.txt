cmake_minimum_required(VERSION 3.26)

project(Geometry
		VERSION 0.0.1
		DESCRIPTION "Geometry Library"
		LANGUAGES CXX)

        
find_package(Eigen3 CONFIG REQUIRED)

set(Sources
    "Source/DataTypes/Mesh.cpp"
    "Source/Nodes/Plane.cpp"
    "Source/Commands/PlaneCommands.cpp"
    )

set(Public_Headers
    "Include/Geometry/DataTypes/Mesh.hpp"
    "Include/Geometry/Nodes/Plane.hpp"
    "Include/Geometry/Commands/PlaneCommands.hpp"
    )

set(Private_Headers
    )

add_library(${PROJECT_NAME} ${Sources} ${Public_Headers} ${Private_Headers})

target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}/DataTypes)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}/Nodes)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}/Commands)



target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} PUBLIC
                        Core
                        Eigen3::Eigen
                     )


#generate_documentation(TargetName)
