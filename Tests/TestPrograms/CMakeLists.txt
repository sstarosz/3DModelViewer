
project(TestPrograms
    VERSION 1.0
    DESCRIPTION "Test programs for the Renderer library"
    LANGUAGES CXX
)





set(Sources
    "SimpleScene.cpp"
)


add_executable(${PROJECT_NAME} ${Sources})


target_link_libraries(${PROJECT_NAME} PRIVATE 
                                            Core 
                                            Geometry
                                            Renderer
                                            Ui
                                            Qt6::Widgets
                                            )

Deploy_QT(${PROJECT_NAME})



set(Sources2
    "NodeGraphPrototype.cpp"
)

add_executable(NodeGraphPrototype ${Sources2})

target_link_libraries(NodeGraphPrototype PRIVATE 
                                            Core 
                                            Geometry
                                            Renderer
                                            )