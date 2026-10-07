cmake_minimum_required(VERSION 3.24)
project(TuskEngine)

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
macro(compile_shaders shader_dir_in)

    #Find all hlsl files. Saves whole directory in string
    file(GLOB 
        HLSL_SHADERS
        CONFIGURE_DEPENDS
        "${shader_dir_in}/*.hlsl"
    )    

    #Output HLSL GLOB STATUS
    message(HLSL_GLOB_STATUS " HLSL Shaders Found: ")
    foreach(SHADER ${HLSL_SHADERS})
        message(HLSL_GLOB_STATUS " ${SHADER}")
    endforeach()

    find_program(
        sc_executable 
        shadercross 
        REQUIRED
        HINTS "Engine/Tools/SDL_shadercross"
    )

    message(SC_EXECUTABLE_STATUS " Shadercross Found: ")
    foreach(PROGRAM_ ${sc_executable})
        message(SC_EXECUTABLE_STATUS " ${PROGRAM_}")
    endforeach()

    set(shader_dir_out ${CMAKE_CURRENT_BINARY_DIR}/Shaders)
    #message("${shader_dir_out}")

    foreach(SHADER ${HLSL_SHADERS})
    #set(SHADEROUT 
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

compile_shaders("Engine/Shaders")