cmake_minimum_required(VERSION 3.26)

project(Renderer
		VERSION 0.0.1
		DESCRIPTION "Rendering Engine "
		LANGUAGES CXX)

        

set(Sources
    "Source/Renderer.cpp"
    "Source/PhysicalDevice.cpp"
    "Source/QueueFamily.cpp"
    "Source/GraphicsPiplineFactory.cpp"
    "Source/Nodes/StandardMaterial.cpp"
    "Source/Commands/StandardMaterialCommands.cpp"
    "Source/Commands/RendererCommands.cpp"
    "Source/Commands/MaterialCommands.cpp"
    )

set(Public_Headers
    "Include/Renderer/Renderer.hpp"
    "Include/Renderer/GraphicsPiplineFactory.hpp"
    "Include/Renderer/VulkanContext.hpp"
    "Include/Renderer/Nodes/StandardMaterial.hpp"
    "Include/Renderer/DataTypes/Renderable.hpp"
    "Include/Renderer/Commands/StandardMaterialCommands.hpp"
    "Include/Renderer/Commands/RendererCommands.hpp"
    "Include/Renderer/Commands/MaterialCommands.hpp"
    )

set(Private_Headers
    "Source/PhysicalDevice.hpp"
    "Source/QueueFamily.hpp"
    )

add_library(${PROJECT_NAME} ${Sources} ${Public_Headers} ${Private_Headers})

target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}/Nodes)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}/DataTypes)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}/Commands)



target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} PUBLIC 
                        Core
                        Geometry
                        Vulkan::Vulkan
                        Vulkan::shaderc_combined
                        Eigen3::Eigen
                        spirv-cross-core 
                        spirv-cross-glsl
                     )



                     
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR NOMINMAX WIN32_LEAN_AND_MEAN)
elseif(UNIX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_XCB_KHR)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_MACOS_MVK)
endif()

#generate_documentation(TargetName)
