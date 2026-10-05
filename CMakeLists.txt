cmake_minimum_required(VERSION 3.24)
project(NeoGrengine)

include(FetchContent)

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
    #add_dependencies(EditorApp StageRuntime)
endif()
