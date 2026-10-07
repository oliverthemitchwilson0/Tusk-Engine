cmake_minimum_required(VERSION 3.24)
project(NeoGrengine)

include(FetchContent)

#C++ Version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#SDL3
set(SDL_SHARED ON CACHE BOOL "Build shared SDL3" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Do not build static SDL3" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Do not build SDL3 test support" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Do not build SDL3 tests" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Do not build SDL3 examples" FORCE)


FetchContent_Declare(
    SDL3 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.16 
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

#Shader Conversion Macro
#https://github.com/TechnicJelle/GPUForBeginners/blob/ab63d5d11df06466a67ec46a5e8cd51723629203/CMakeLists.txt#L35-L68
macro(compileShaders)
    set(shadersOutputDir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders) #sets variable as the location of the shaders in the build

    #Recursivly search for any files ending in .hlsl, adding it to srcShaderFiles 
    file(GLOB_RECURSE srcShaderFiles 
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hlsl"
    )

    file(GLOB scDirs "${CMAKE_SOURCE_DIR}/Engine/Tools/SDL*_shadercross*")
    find_program(
        shadercrossExe 
        shadercross 
        REQUIRED
        HINTS ${scDirs}
    )

    foreach(shader ${srcShaderFiles})
        set(inShader "${CMAKE_CURRENT_SOURCE_DIR}/${shader}")
        set(outShader "${shadersOutputDir}/${shader}")
        string(REPLACE ".hlsl" ".spv" shaderSpv ${outShader})
        string(REPLACE ".hlsl" ".msl" shaderMsl ${outShader})
		string(REPLACE ".hlsl" ".dxil" shaderDxil ${outShader})
        add_custom_command(
				OUTPUT ${outShader}
				COMMAND ${shadercrossExe} ${inShader} -o ${shaderSpv}
				COMMAND ${shadercrossExe} ${inShader} -o ${shaderMsl}
				COMMAND ${shadercrossExe} ${inShader} -o ${shaderDxil}
				MAIN_DEPENDENCY ${inShader}
		)
        list(APPEND shadersOutputDir ${outShader})
    endforeach()

endmacro()

#Engine
add_library(Engine STATIC
    Engine/src/Engine.cpp
    Engine/src/Renderer.cpp
)
add_library(Game STATIC Game/src/Game.cpp)

add_executable(GameApp GameApp/src/GameApp.cpp)

target_include_directories(Engine PUBLIC Engine/include)
target_include_directories(Game PUBLIC Game/include)

target_link_libraries(Engine PRIVATE SDL3::SDL3)
target_link_libraries(Game PUBLIC Engine)
target_link_libraries(GameApp PRIVATE Game)

if(WIN32)
    add_custom_target(StageRuntime
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:GameApp>
        VERBATIM)
    add_dependencies(GameApp StageRuntime)
endif()

compileShaders()