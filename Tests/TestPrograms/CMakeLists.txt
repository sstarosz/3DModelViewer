
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
                                            spdlog::spdlog
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


set(Sources3
    "InputPrototype.cpp"
)

add_executable(InputPrototype ${Sources3})

target_link_libraries(InputPrototype PRIVATE 
                                            Core 
                                            Geometry
                                            Renderer
                                            Ui
                                            Qt6::Widgets
                                            )


set(Sources4 "NodeGraphPrototype2.cpp")
add_executable(NodeGraphPrototype2 ${Sources4})
target_link_libraries(NodeGraphPrototype2 PRIVATE Eigen3::Eigen)
target_compile_features(NodeGraphPrototype2 PRIVATE cxx_std_20)


set(Sources4 "NodeGraphPrototype3.cpp")
add_executable(NodeGraphPrototype3 ${Sources4})
target_link_libraries(NodeGraphPrototype3 PRIVATE Eigen3::Eigen Core)
target_compile_features(NodeGraphPrototype3 PRIVATE cxx_std_20)