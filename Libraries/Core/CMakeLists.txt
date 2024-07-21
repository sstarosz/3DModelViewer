cmake_minimum_required(VERSION 3.26)

project(Core
		VERSION 0.0.1
		DESCRIPTION "Rendering Engine "
		LANGUAGES CXX)

        

set(Sources
    "Source/Nodes/Camera.cpp"
    "Source/Transform.cpp"
    "Source/StObject.cpp"
    "Source/ContentManager.cpp"
    "Source/NodeGraph.cpp"
    "Source/Attribute.cpp"
    "Source/NumericAttribute.cpp"
    )

set(Public_Headers
    "Include/Core/Nodes/Camera.hpp"
    "Include/Core/Transform.hpp"
    "Include/Core/StObject.hpp"
    "Include/Core/DataTypes/DataTypes.hpp"
    "Include/Core/ContentManager.hpp"
    "Include/Core/Node.hpp"
    "Include/Core/NodeGraph.hpp"
    "Include/Core/Attribute.hpp"
    "Include/Core/NumericAttribute.hpp"
    )

set(Private_Headers

    )

add_library(${PROJECT_NAME} ${Sources} ${Public_Headers} ${Private_Headers})

target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}/Nodes)



target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen spdlog::spdlog)



                     


#generate_documentation(TargetName)
