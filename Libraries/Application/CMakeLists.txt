cmake_minimum_required(VERSION 3.26)

project(Application
		VERSION 0.0.1
		DESCRIPTION "Rendering Engine "
		LANGUAGES CXX)

        

set(Sources
    "Source/Creator.cpp"
    "Source/Modifier.cpp"
    "Source/Application.cpp"
    )

set(Public_Headers
    "Include/Application/Creator.hpp"
    "Include/Application/Modifier.hpp"
    "Include/Application/Application.hpp"
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

target_link_libraries(${PROJECT_NAME} PUBLIC 
                                                Core
                                                Geometry
                                                Renderer
                                                Ui
                                                Qt6::Widgets #TODO remove this in the future
                        )



                     


#generate_documentation(TargetName)
