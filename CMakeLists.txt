cmake_minimum_required(VERSION 3.20)

project(MOONSHINE_RUNNERS VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")

option(BUILD_SCRIPT_TOOL "Build script tool" OFF)

set(APPLICATION_NAME moonshine-runners)

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
# Subsystems the game doesn't use: miniaudio plays the sound and sokol draws.
set(SDL_AUDIO OFF)
set(SDL_GPU OFF)
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

add_subdirectory(MechoSoma)

if(BUILD_SCRIPT_TOOL)
    add_subdirectory(ScriptTool)
endif(BUILD_SCRIPT_TOOL)

set_target_properties(${APPLICATION_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
