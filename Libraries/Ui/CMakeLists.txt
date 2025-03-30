cmake_minimum_required(VERSION 3.26)

project(Ui
		VERSION 0.0.1
		DESCRIPTION "Ui library"
		LANGUAGES CXX)

        
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



set(Sources
    "Source/MainWindow.cpp"
    "Source/Viewport.cpp"
    "Source/NodeEditor.cpp"
    "Source/GuiManager.cpp"
    "Source/AttributeEditor.cpp"
    )

set(Public_Headers
    "Include/Ui/MainWindow.hpp"
    "Include/Ui/Viewport.hpp"
    "Include/Ui/NodeEditor.hpp"
    "Include/Ui/GuiManager.hpp"
    "Include/Ui/AttributeEditor.hpp"
    )

set(Private_Headers)




add_library(${PROJECT_NAME} ${Sources} ${Public_Headers} ${Private_Headers})

target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/Ui)



target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} PRIVATE
                        Qt6::Widgets
                        Qt6::OpenGLWidgets
                        Renderer
                        Core 
                        Vulkan::Vulkan
                        spdlog::spdlog
                     )


#generate_documentation(TargetName)
