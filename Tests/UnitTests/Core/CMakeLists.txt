
project(CoreTests)


set(Test_Library "Core")
set(Test_Name "CoreTests")


set(Tests_Sources
    "CameraTest.cpp"
)


include(${CMAKE_SOURCE_DIR}/Tests/BuildUnitTests.cmake)


target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/Libraries/${Test_Library}/Include/${Test_Library}
        ${CMAKE_SOURCE_DIR}/Libraries/${Test_Library}/Source/)