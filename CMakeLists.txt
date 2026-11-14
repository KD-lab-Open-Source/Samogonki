cmake_minimum_required(VERSION 3.20)

project(MOONSHINE_RUNNERS VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")

option(BUILD_SCRIPT_TOOL "Build script tool" OFF)

set(APPLICATION_NAME moonshine-runners)

# Build identification shown in crash reports.
if(NOT DEFINED GIT_BRANCH)
    if(DEFINED ENV{GITHUB_REF_NAME} AND NOT "$ENV{GITHUB_REF_NAME}" STREQUAL "")
        set(GIT_BRANCH "$ENV{GITHUB_REF_NAME}")
    else()
        execute_process(
            COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET)
    endif()
endif()

if(NOT DEFINED GIT_COMMIT_HASH)
    if(DEFINED ENV{GITHUB_SHA} AND NOT "$ENV{GITHUB_SHA}" STREQUAL "")
        set(GIT_COMMIT_HASH "$ENV{GITHUB_SHA}")
    else()
        execute_process(
            COMMAND git rev-parse --short HEAD
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET)
    endif()
endif()

if(NOT GIT_BRANCH)
    set(GIT_BRANCH "unknown")
endif()
if(NOT GIT_COMMIT_HASH)
    set(GIT_COMMIT_HASH "unknown")
endif()
add_compile_definitions(GIT_BRANCH="${GIT_BRANCH}" GIT_COMMIT_HASH="${GIT_COMMIT_HASH}")

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()

add_definitions(-D_MECHOSOMA_ -D_USE_TILEMAP_ -D_USE_TERRA_ -DNASSERT -D_FINAL_VERSION_ -D_MPPLUS_SOUNDTRACK_)

if (LINUX)
    add_definitions(-D__forceinline=inline -Dstricmp=strcasecmp -D_DEBUG)

    add_custom_target(sokol-shaders
            COMMAND "${CMAKE_CURRENT_LIST_DIR}/3rdparty/sokol-tools-bin/bin/linux/sokol-shdc"
            --input "${CMAKE_CURRENT_LIST_DIR}/MechoSoma/app/sokol/shader.glsl"
            --output "${CMAKE_CURRENT_LIST_DIR}/MechoSoma/app/sokol-shader.h"
            --slang glsl330)
endif (LINUX)

add_compile_options(-fsigned-char)
# add_compile_options(-Werror=shift-negative-value)
# add_compile_options(-Werror=dangling-else)
# add_compile_options(-Werror=parentheses)
add_compile_options(-Werror=delete-non-virtual-dtor)
add_compile_options(-Werror=return-type)
add_compile_options(-Werror=write-strings)
add_compile_options(-Werror=delete-incomplete)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Werror=mismatched-new-delete)
endif ()

add_subdirectory(3rdparty/3dsftk3)
add_subdirectory(3rdparty/iniparser)
add_subdirectory(3rdparty/musepack_src_r475)

add_subdirectory(MechoSoma)

if(BUILD_SCRIPT_TOOL)
    add_subdirectory(ScriptTool)
endif(BUILD_SCRIPT_TOOL)

set_target_properties(${APPLICATION_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
