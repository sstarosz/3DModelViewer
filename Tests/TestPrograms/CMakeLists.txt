
project(TestPrograms
    VERSION 1.0
    DESCRIPTION "Test programs for the Renderer library"
    LANGUAGES CXX
)





set(Sources
    "SimpleScene.cpp"
)


add_executable(${PROJECT_NAME} ${Sources})


target_link_libraries(${PROJECT_NAME} PRIVATE Renderer)
