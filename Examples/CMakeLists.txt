project(Examples
    VERSION 1.0
    DESCRIPTION "Examples for the ModelViewer project"
    LANGUAGES CXX
)

set(Sources "SimpleScene.cpp")

add_executable(${PROJECT_NAME} ${Sources})

target_link_libraries(${PROJECT_NAME} PRIVATE Application)

Deploy_QT(${PROJECT_NAME})


set(DebugSources "DebugScene.cpp")

add_executable(DebugScene ${DebugSources})

target_link_libraries(DebugScene PRIVATE Application)