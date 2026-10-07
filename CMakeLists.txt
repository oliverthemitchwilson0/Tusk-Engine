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
macro(compile_shaders)
    set(SHADERS_INPUT_DIR ${CMAKE_SOURCE_DIR}/Engine/Shaders)
    set(SHADERS_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders)

    file(GLOB_RECURSE src_shader_files
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}"${SHADERS_INPUT_DIR}/*.hlsl"
    )

    file(GLOB SC_DIRS "${CMAKE_SOURCE_DIR}/Engine/Tools/SDL*_shadercross*")
    find_program(
        SC_EXECUTABLE 
        shadercross 
        REQUIRED
        HINTS ${SC_DIRS}
        PATH_SUFFIXES bin 
    )
    
    foreach (fn ${src_shader_files})
		set(in_f "${CMAKE_CURRENT_SOURCE_DIR}/${fn}")
		set(out_f "${SHADERS_OUTPUT_DIR}/${fn}")
		string(REPLACE ".hlsl" ".spv" fn_spv ${out_f})
		string(REPLACE ".hlsl" ".msl" fn_msl ${out_f})
		string(REPLACE ".hlsl" ".dxil" fn_dxil ${out_f})
		add_custom_command(
				OUTPUT ${out_f}
				COMMAND ${SC_EXECUTABLE} ${in_f} -o ${fn_spv}
				COMMAND ${SC_EXECUTABLE} ${in_f} -o ${fn_msl}
				COMMAND ${SC_EXECUTABLE} ${in_f} -o ${fn_dxil}
				MAIN_DEPENDENCY ${in_f}
		)
		list(APPEND out_shader_files ${out_f})
	endforeach ()

	add_custom_target(${PROJECT_NAME}_shaders DEPENDS ${out_shader_files})

	add_dependencies(Game ${PROJECT_NAME}_shaders)

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

compile_shaders()