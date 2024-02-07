cmake_minimum_required(VERSION 3.26)

project(ModelViewer
        VERSION 0.0.1 
        DESCRIPTION  "3d Model Viewer" 
        LANGUAGES  CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


#---------------------------------------------------------------------------------------#
#----------------------------------Project option---------------------------------------#
#---------------------------------------------------------------------------------------#
option(Build_Docs           "Determine if build documentation" OFF)
option(Build_Tests          "Determine if build unit tests" ON)
option(Build_Benchmarks     "Determine if build benchmarks" OFF)


option(Enable_ASAN          "Build with AddressSanitizer" OFF) #TODO


#---------------------------------------------------------------------------------------#
#----------------------------------Project configuration-------------------------------#
#---------------------------------------------------------------------------------------#
include(CMake/Deploy.cmake)


#---------------------------------------------------------------------------------------#
#----------------------------------Project dependicies----------------------------------#
#---------------------------------------------------------------------------------------#
find_package(Qt6 CONFIG REQUIRED
             COMPONENTS Core
                        Gui
                        Widgets
                        CoreTools
            )

find_package(Stb REQUIRED)
find_package(spirv_cross_core  CONFIG REQUIRED)
find_package(spirv_cross_glsl  CONFIG REQUIRED)



#---------------------------------------------------------------------------------------#
#----------------------------------Project Structure------------------------------------#
#---------------------------------------------------------------------------------------#
add_subdirectory(Libraries)
add_subdirectory(Application)
add_subdirectory(Assets)

if(Build_Docs)
    add_subdirectory(Docs)
endif()

if(Build_Benchmarks)
    add_subdirectory(Benchmarks)
endif()

if(Build_Tests)
    add_subdirectory(Tests)
endif()

