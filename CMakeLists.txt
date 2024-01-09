cmake_minimum_required(VERSION 3.12)

project(MOONSHINE_RUNNERS VERSION 1.0.0 LANGUAGES C CXX)

set(APPLICATION_NAME moonshine-runners)

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

set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_USE_CPP14 ON CACHE INTERNAL "" FORCE)
add_subdirectory(3rdparty/tinygltf)

add_subdirectory(3rdparty/iniparser)

add_subdirectory(MechoSoma)

set_target_properties(${APPLICATION_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
