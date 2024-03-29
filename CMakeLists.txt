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
option(Build_Tests_For_Gpu  "Determine if build unit that use Vulkan or other gpu features" ON)
option(Build_Benchmarks     "Determine if build benchmarks" OFF)


option(Enable_ASAN          "Build with AddressSanitizer" OFF) #TODO


#---------------------------------------------------------------------------------------#
#----------------------------------Project configuration-------------------------------#
#---------------------------------------------------------------------------------------#
include(CMake/Deploy.cmake)

if(Build_Tests)
    enable_testing()
endif()


#---------------------------------------------------------------------------------------#
#----------------------------------Project flags----------------------------------------#
#---------------------------------------------------------------------------------------#
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    message(STATUS "MSVC detected")
    add_compile_options(/W4)            #Enable all warnings
#   add_compile_options(/WX)            #Treat warnings as errors    
    add_compile_options(/permissive-)   #Disable language extensions
    add_compile_options(/EHsc)          #Enable C++ exception handling
else()
    add_compile_options(-Wall)          #Enable all warnings
    add_compile_options(-Wextra)        #Enable extra warnings
    add_compile_options(-Wpedantic)     #Enable pedantic warnings
#   add_compile_options(-Werror)        #Treat warnings as errors
endif()


#---------------------------------------------------------------------------------------#
#----------------------------------Project dependicies----------------------------------#
#---------------------------------------------------------------------------------------#
find_package(Qt6 CONFIG REQUIRED
             COMPONENTS Core
                        Gui
                        Widgets
                        CoreTools
            )

find_package(Vulkan REQUIRED
                COMPONENTS  glslc 
                            shaderc_combined
            )

find_package(Stb                      REQUIRED)
find_package(spirv_cross_core  CONFIG REQUIRED)
find_package(spirv_cross_glsl  CONFIG REQUIRED)
find_package(Eigen3            CONFIG REQUIRED)



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

