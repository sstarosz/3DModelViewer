
project(GeometryTests)


set(Test_Library "Geometry")
set(Test_Name "GeometryTests")


set(Tests_Sources
    "MeshTest.cpp"
    "PlaneTest.cpp"
)


include(${CMAKE_SOURCE_DIR}/Tests/BuildUnitTests.cmake)


target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/Libraries/${Test_Library}/Include/${Test_Library}
        ${CMAKE_SOURCE_DIR}/Libraries/${Test_Library}/Include/${Test_Library}/DataTypes/
        ${CMAKE_SOURCE_DIR}/Libraries/${Test_Library}/Source/)