cmake_minimum_required(VERSION 3.25)

# The oldest macOS the game runs on, on Apple Silicon and Intel alike: the Steam client's
# own minimum, macOS 12 Monterey since October 2025. Without it the compiler targets the
# macOS of the machine that builds the game, and the game doesn't start on anything older.
# It has to be set before project(); -DCMAKE_OSX_DEPLOYMENT_TARGET=... or the
# MACOSX_DEPLOYMENT_TARGET environment variable still give another one.
if (NOT CMAKE_OSX_DEPLOYMENT_TARGET AND NOT DEFINED ENV{MACOSX_DEPLOYMENT_TARGET})
    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "The oldest macOS the game runs on" FORCE)
endif ()

project(MOONSHINE_RUNNERS VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")

option(BUILD_SCRIPT_TOOL "Build script tool" OFF)

set(APPLICATION_NAME moonshine-runners)

# The SDL GPU renderer: Metal on macOS, Direct3D 12 on Windows, Vulkan on Linux. It needs
# a shader compiler at build time (see below). The web build has no SDL GPU backend yet
# and draws with sokol (WebGL 2), as the desktop builds do without this.
if (EMSCRIPTEN)
    set(GPU_RENDERER OFF)
else ()
    option(GPU_RENDERER "Build the SDL GPU renderer (needs a shader compiler, see README)" ON)
endif ()

if (MSVC)
    # The C and C++ runtimes linked in, like everything else, so the game needs no Visual
    # C++ redistributable. It has to be set before any library below is defined.
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # RelWithDebInfo would only inline functions marked inline; inline what Release does.
    foreach(_lang C CXX)
        string(REPLACE "/Ob1" "/Ob2" CMAKE_${_lang}_FLAGS_RELWITHDEBINFO "${CMAKE_${_lang}_FLAGS_RELWITHDEBINFO}")
    endforeach()
endif ()

# SDL 3, downloaded at configure time and built with the game as a static library
# (SDL3::SDL3-static). It comes before the game's global definitions and warning flags
# below, which a subdirectory added after them would inherit. To build without a
# download, pass -DFETCHCONTENT_SOURCE_DIR_SDL3=<an SDL 3.4.20 source tree>.
include(FetchContent)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_TEST_LIBRARY OFF)
# Subsystems the game doesn't use: miniaudio plays the sound, and sokol or SDL GPU draws,
# not SDL's 2D renderer.
set(SDL_AUDIO OFF)
set(SDL_GPU ${GPU_RENDERER})
set(SDL_RENDER OFF)
set(SDL_CAMERA OFF)
set(SDL_DIALOG OFF)
set(SDL_TRAY OFF)
FetchContent_Declare(SDL3
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.20/SDL3-3.4.20.tar.gz
    URL_HASH SHA256=f3bb06f55bed9557227a700af2ea62e49497ebdf58037732a731a5fb3ef8581f
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(SDL3)

# Windows has no zlib of its own: it is fetched and built static as well, as ZLIB::ZLIB,
# the target find_package(ZLIB) gives the other platforms.
if (WIN32)
    set(ZLIB_BUILD_TESTING OFF)
    set(ZLIB_BUILD_SHARED OFF)
    set(ZLIB_BUILD_STATIC ON)
    set(ZLIB_INSTALL OFF)
    FetchContent_Declare(zlib
        URL https://github.com/madler/zlib/releases/download/v1.3.2/zlib-1.3.2.tar.gz
        URL_HASH SHA256=bb329a0a2cd0274d05519d61c667c062e06990d72e125ee2dfa8de64f0119d16
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(zlib)
    add_library(ZLIB::ZLIB ALIAS zlibstatic)
endif ()

# The SDL GPU renderer's shader compiler. The HLSL in MechoSoma/app/sdl_gpu/shaders is
# compiled at build time to the one format this platform's backend loads, and embedded in
# the game (cmake/compile_shaders.cmake):
#
#   Windows: Direct3D 12 -> DXIL,   with the Windows SDK's dxc.exe
#   macOS:   Metal       -> MSL,    with SDL_shadercross
#   Linux:   Vulkan      -> SPIR-V, with SDL_shadercross
#
# The toolchain is VistaEngine's (github.com/KD-lab-Open-Source/VistaEngine). SDL_shadercross
# is built here, with DXC and SPIRV-Cross under it. Linux downloads Microsoft's prebuilt DXC
# (it needs glibc 2.38); macOS has none, so DXC is built from source, which takes a while.
# -DSHADERCROSS_EXECUTABLE=<a built shadercross> skips all of that: the macOS CI caches one.
#
# -DGPU_SHADER_HEADER_PREBUILT=<sdl_gpu_shaders.h> takes the shaders compiled by another
# build (its gpu_shaders target, for the same platform), and this build needs no shader
# compiler at all. The Steam Linux build uses it: it is made on an older Ubuntu for its
# glibc, where Microsoft's prebuilt DXC doesn't link.
set(GPU_SHADER_HEADER_PREBUILT "" CACHE FILEPATH "SDL GPU shaders compiled by another build's gpu_shaders target")
if (GPU_RENDERER)
    if (WIN32)
        set(SHADER_FORMAT DXIL)
    elseif (APPLE)
        set(SHADER_FORMAT MSL)
    else ()
        set(SHADER_FORMAT SPIRV)
    endif ()
    if (GPU_SHADER_HEADER_PREBUILT)
        if (NOT EXISTS "${GPU_SHADER_HEADER_PREBUILT}")
            message(FATAL_ERROR "GPU_SHADER_HEADER_PREBUILT=${GPU_SHADER_HEADER_PREBUILT} does not exist")
        endif ()
        # compile_shaders.cmake names the format on the header's first line.
        file(STRINGS "${GPU_SHADER_HEADER_PREBUILT}" _prebuilt_first_line LIMIT_COUNT 1)
        if (NOT _prebuilt_first_line MATCHES ": ${SHADER_FORMAT}\\.$")
            message(FATAL_ERROR "GPU_SHADER_HEADER_PREBUILT=${GPU_SHADER_HEADER_PREBUILT} is not ${SHADER_FORMAT}, "
                "this platform's format: '${_prebuilt_first_line}'")
        endif ()
        message(STATUS "Shaders: ${SHADER_FORMAT}, prebuilt in ${GPU_SHADER_HEADER_PREBUILT}")
    elseif (WIN32)
        # dxc.exe comes with the Windows SDK, under Windows Kits\10\bin\<version>\x64. Take
        # the newest SDK's.
        if (NOT DXC_EXECUTABLE)
            file(GLOB _dxc_candidates
                "C:/Program Files (x86)/Windows Kits/10/bin/*/x64/dxc.exe"
                "C:/Program Files/Windows Kits/10/bin/*/x64/dxc.exe")
            if (_dxc_candidates)
                list(SORT _dxc_candidates COMPARE NATURAL ORDER DESCENDING)
                list(GET _dxc_candidates 0 DXC_EXECUTABLE)
            endif ()
        endif ()
        if (NOT DXC_EXECUTABLE)
            find_program(DXC_EXECUTABLE NAMES dxc.exe dxc HINTS "$ENV{WindowsSdkVerBinPath}x64")
        endif ()
        if (NOT DXC_EXECUTABLE)
            message(FATAL_ERROR "dxc.exe not found. Install the Windows SDK, pass -DDXC_EXECUTABLE=<dxc.exe>, "
                "or build without the SDL GPU renderer (-DGPU_RENDERER=OFF).")
        endif ()
        set(DXC_EXECUTABLE "${DXC_EXECUTABLE}" CACHE FILEPATH "The Windows SDK's dxc.exe (HLSL to DXIL)" FORCE)
        message(STATUS "Shaders: DXIL, with ${DXC_EXECUTABLE}")
        set(SHADER_MODE dxc)
        set(SHADER_COMPILER "${DXC_EXECUTABLE}")
    else ()
        set(SHADER_MODE shadercross)

        if (SHADERCROSS_EXECUTABLE)
            if (NOT EXISTS "${SHADERCROSS_EXECUTABLE}")
                message(FATAL_ERROR "SHADERCROSS_EXECUTABLE=${SHADERCROSS_EXECUTABLE} does not exist")
            endif ()
            set(SHADER_COMPILER "${SHADERCROSS_EXECUTABLE}")
            message(STATUS "Shaders: ${SHADER_FORMAT}, with ${SHADERCROSS_EXECUTABLE}")
        else ()
            # The CI caches its macOS build under this revision; change both together.
            set(SHADERCROSS_REVISION 6b06e55c7c5d7e7a09a8a14f76e866dcfad5ab99)
            # LINUX is CMake's own (3.25): on the web build, the definition further down
            # makes it true too, but this part isn't built there.
            if (LINUX)
                set(_prebuilt_dxc TRUE)
                set(_shadercross_submodules GIT_SUBMODULES external/SPIRV-Cross)
            else ()
                set(_prebuilt_dxc FALSE)
                set(_shadercross_submodules)   # no GIT_SUBMODULES: all of them, DXC included
            endif ()
            # SOURCE_SUBDIR names a directory that doesn't exist, so that FetchContent only
            # downloads: the options below and Linux's DXC download come before its CMake.
            FetchContent_Declare(SDL_shadercross
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_shadercross.git
                GIT_TAG ${SHADERCROSS_REVISION}
                ${_shadercross_submodules}
                SOURCE_SUBDIR download-only)
            FetchContent_MakeAvailable(SDL_shadercross)

            set(SDLSHADERCROSS_DXC ON CACHE BOOL "" FORCE)
            set(SDLSHADERCROSS_STATIC ON CACHE BOOL "" FORCE)
            set(SDLSHADERCROSS_SHARED OFF CACHE BOOL "" FORCE)
            set(SDLSHADERCROSS_CLI ON CACHE BOOL "" FORCE)
            set(SDLSHADERCROSS_CLI_STATIC ON CACHE BOOL "" FORCE)   # with the game's static SDL
            set(SDLSHADERCROSS_INSTALL OFF CACHE BOOL "" FORCE)
            set(SDLSHADERCROSS_TESTS OFF CACHE BOOL "" FORCE)
            set(SDLSHADERCROSS_SPIRVCROSS_SHARED OFF CACHE BOOL "" FORCE)
            set(CMAKE_POSITION_INDEPENDENT_CODE ON)   # static libraries inside the CLI
            if (_prebuilt_dxc)
                # Puts Microsoft's DXC under external/DirectXShaderCompiler-binaries, where
                # SDL_shadercross looks for it.
                include("${sdl_shadercross_SOURCE_DIR}/build-scripts/download-prebuilt-DirectXShaderCompiler.cmake")
                # SDLSHADERCROSS_VENDORED would build DXC as well as SPIRV-Cross, so
                # SPIRV-Cross is added here instead; with its target defined, SDL_shadercross
                # doesn't look for an installed one. Its install rules stay on, because
                # SDL_shadercross exports its own static library, which links it.
                set(SDLSHADERCROSS_VENDORED OFF CACHE BOOL "" FORCE)
                set(SPIRV_CROSS_SKIP_INSTALL OFF CACHE BOOL "" FORCE)
                set(SPIRV_CROSS_CLI OFF CACHE BOOL "" FORCE)
                set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
                set(SPIRV_CROSS_SHARED OFF CACHE BOOL "" FORCE)
                set(SPIRV_CROSS_STATIC ON CACHE BOOL "" FORCE)
                add_subdirectory("${sdl_shadercross_SOURCE_DIR}/external/SPIRV-Cross"
                    "${sdl_shadercross_BINARY_DIR}/external/SPIRV-Cross" EXCLUDE_FROM_ALL)
                add_library(spirv_cross_c ALIAS spirv-cross-c)
            else ()
                set(SDLSHADERCROSS_VENDORED ON CACHE BOOL "" FORCE)
                set(ENABLE_SPIRV_CODEGEN ON CACHE BOOL "" FORCE)   # DXC leaves it off
            endif ()
            # shadercross only runs on the machine that builds the game, so it is built for
            # that machine's architecture alone, even when the game is built for both
            # (-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"), which would build DXC twice.
            if (APPLE)
                set(CMAKE_OSX_ARCHITECTURES "${CMAKE_HOST_SYSTEM_PROCESSOR}")
            endif ()
            add_subdirectory("${sdl_shadercross_SOURCE_DIR}" "${sdl_shadercross_BINARY_DIR}" EXCLUDE_FROM_ALL)
            if (APPLE)
                unset(CMAKE_OSX_ARCHITECTURES)   # the game's again, from the cache
            endif ()
            unset(CMAKE_POSITION_INDEPENDENT_CODE)
            # Resolved at build time: the shadercross this build makes.
            set(SHADER_COMPILER "$<TARGET_FILE:shadercross>")
            message(STATUS "Shaders: ${SHADER_FORMAT}, with SDL_shadercross built from source")
        endif ()
    endif ()
endif ()

# Build identification for the log and crash reports: build_info.h, with GIT_BRANCH and
# GIT_COMMIT_HASH, made at every build by cmake/build_info.cmake. -DGIT_BRANCH=... and
# -DGIT_COMMIT_HASH=... set them instead.
set(BUILD_INFO_DIR "${CMAKE_BINARY_DIR}/generated/build_info")
add_custom_target(build_info
    COMMAND "${CMAKE_COMMAND}"
        "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
        "-DOUTPUT=${BUILD_INFO_DIR}/build_info.h"
        "-DBRANCH=${GIT_BRANCH}"
        "-DCOMMIT=${GIT_COMMIT_HASH}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_info.cmake"
    BYPRODUCTS "${BUILD_INFO_DIR}/build_info.h"
    COMMENT "Build identification"
    VERBATIM)

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()

add_definitions(-D_MECHOSOMA_ -D_USE_TILEMAP_ -D_USE_TERRA_ -DNASSERT -D_FINAL_VERSION_ -D_MPPLUS_SOUNDTRACK_)

if (LINUX)
    add_definitions(-D__forceinline=inline -Dstricmp=strcasecmp -D_DEBUG)
endif (LINUX)

if (MSVC)
    # char is already signed with MSVC. The POSIX names (stricmp, strdup) and the classic
    # CRT functions stay without deprecation warnings, math.h defines M_PI, and windows.h
    # (which miniaudio includes) defines no min/max macros to break std::min and the game's
    # own min/max.
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS _USE_MATH_DEFINES NOMINMAX)
    # The sources are UTF-8, and their strings stay UTF-8 in the program, as with GCC and
    # Clang. Without this MSVC reads them in the system's code page. (Text drawn with the
    # game's CP1251 fonts is written as \x escapes.)
    add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/utf-8> $<$<COMPILE_LANGUAGE:C,CXX>:/bigobj>)
    # Standard C++ rules, as GCC and Clang apply them. Permissive MSVC also finds friend
    # functions declared only inside a class, so overloads can resolve differently: it
    # picked xmath's Vect3d stream operator, never compiled anywhere, in pt.cpp.
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/permissive-> $<$<COMPILE_LANGUAGE:CXX>:/Zc:hiddenFriend>)
    if (CMAKE_GENERATOR MATCHES "Visual Studio")
        # Compile a project's files in parallel; Ninja does that by itself.
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/MP>)
    endif ()
else ()
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
endif ()

add_subdirectory(3rdparty/3dsftk3)
add_subdirectory(3rdparty/iniparser)
add_subdirectory(3rdparty/musepack_src_r475)

# A target whose sources include build_info.h.
function(use_build_info target)
    add_dependencies(${target} build_info)
    target_include_directories(${target} PRIVATE "${BUILD_INFO_DIR}")
endfunction()

add_subdirectory(MechoSoma)
use_build_info(${APPLICATION_NAME})

if(BUILD_SCRIPT_TOOL)
    add_subdirectory(ScriptTool)
endif(BUILD_SCRIPT_TOOL)

set_target_properties(${APPLICATION_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
