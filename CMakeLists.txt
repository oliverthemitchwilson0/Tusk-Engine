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

#GLM

FetchContent_Declare(
    GLM
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(GLM)

#Shader Conversion Macro
#https://github.com/TechnicJelle/GPUForBeginners/blob/ab63d5d11df06466a67ec46a5e8cd51723629203/CMakeLists.txt#L35-L68
macro(compile_shaders shader_dir_in)

    #Find all hlsl files.
    file(GLOB 
        HLSL_SHADERS
        CONFIGURE_DEPENDS
        RELATIVE ${shader_dir_in}/
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

    foreach(FN ${HLSL_SHADERS})
        set(FILE_IN "${shader_dir_in}/${FN}")
        set(FILE_OUT "${shader_dir_out}/${FN}")
        
        #Create the output files
        string(REPLACE ".hlsl" ".spv" FN_SPV ${FILE_OUT})
		string(REPLACE ".hlsl" ".msl" FN_MSL ${FILE_OUT})
		string(REPLACE ".hlsl" ".dxil" FN_DXIL ${FILE_OUT})

        #Create Custom Command - called automatically
        add_custom_command(
            OUTPUT #Define the command's output
                "${FN_SPV}" 
                "${FN_MSL}"
                "${FN_DXIL}"
            
            COMMAND #Create shader compatibility
                "${CMAKE_COMMAND}" -E make_directory 
                "${shader_dir_out}"

            #Shadercross commands
            COMMAND 
                "${sc_executable}"
                "${FILE_IN}" 
                -o 
                "${FN_SPV}" 

            COMMAND 
                "${sc_executable}" 
                "${FILE_IN}" 
                -o 
                "${FN_MSL}" 

            COMMAND 
                "${sc_executable}"
                "${FILE_IN}" 
                -o 
                "${FN_DXIL}" 

            DEPENDS 
                "${FILE_IN}"

            VERBATIM #Follow commands exactly
        )
        list(APPEND compiled_shaders "${FN_SPV}" "${FN_MSL}" "${FN_DXIL}") #Add outputs to compiled shaderslist

    endforeach()

    add_custom_target( #Creates a target that defines TUSKShaders is complete when all files are generated?
        TUSKShaders
        ALL
        DEPENDS
        ${compiled_shaders}
    )

    add_dependencies( #Build TuskShaders before Game
        Game
        TUSKShaders
    )

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

target_link_libraries(Engine PRIVATE 
    SDL3::SDL3
    glm::glm
)
target_link_libraries(Game PUBLIC Engine)
target_link_libraries(GameApp PRIVATE Game)

if(WIN32)
    add_custom_target(StageRuntime
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:GameApp>
        VERBATIM)
    add_dependencies(GameApp StageRuntime)
endif()

compile_shaders("${CMAKE_SOURCE_DIR}/Engine/Shaders")