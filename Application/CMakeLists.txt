cmake_minimum_required(VERSION 3.24)

project(StModelViewerApp 
        VERSION 0.0.1
 		DESCRIPTION "Main application which is the entry point of aplication" 
        LANGUAGES CXX)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(Headers)
set(Sources "main.cpp")


add_executable(${PROJECT_NAME} ${Headers} ${Sources})



target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})
target_link_libraries(${PROJECT_NAME} PRIVATE
                        Qt6::Widgets
                        Ui
                        Core
                        )


                        
#Copy Qt dlls to output directory
Deploy_QT(${PROJECT_NAME})

