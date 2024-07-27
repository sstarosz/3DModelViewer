
project(RendererTests)


set(Test_Library "Renderer")
set(Test_Name "RendererTests")


set(Tests_Sources
    "GraphicsPiplineFactoryTest.cpp"
)


include(${CMAKE_SOURCE_DIR}/Tests/BuildUnitTests.cmake)


target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/Libraries/${Test_Library}/Include/${Test_Library}
        ${CMAKE_SOURCE_DIR}/Libraries/${Test_Library}/Source/)