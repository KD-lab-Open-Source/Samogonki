cmake_minimum_required(VERSION 3.20)

project(SCRIPT_TOOL VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")

set(APPLICATION_NAME script-tool)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG -DXSTREAM_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

find_package(ZLIB REQUIRED)
find_package(Iconv REQUIRED)

get_filename_component(PARENT_DIR ../ ABSOLUTE)

set(SOURCES
    src/common.cpp
    src/common.h
    src/localization_file.cpp
    src/localization_file.h
    src/localization.cpp
    src/localization.h
    src/main.cpp
    src/text_converter.cpp
    src/text_converter.h
    # ParserC
    ${PARENT_DIR}/MechoSoma/ParserC/math_exp.cpp
    ${PARENT_DIR}/MechoSoma/ParserC/prepare.cpp
    ${PARENT_DIR}/MechoSoma/ParserC/script_err.cpp
    ${PARENT_DIR}/MechoSoma/ParserC/script.cpp
    # XTool
    ${PARENT_DIR}/MechoSoma/XTool/filesystem.cpp
    ${PARENT_DIR}/MechoSoma/XTool/inzip.cpp
    ${PARENT_DIR}/MechoSoma/XTool/itoa.cpp
    ${PARENT_DIR}/MechoSoma/XTool/outzip.cpp
    ${PARENT_DIR}/MechoSoma/XTool/treezip.cpp
    ${PARENT_DIR}/MechoSoma/XTool/xbcnvin.cpp
    ${PARENT_DIR}/MechoSoma/XTool/xbcnvout.cpp
    ${PARENT_DIR}/MechoSoma/XTool/xbcore.cpp
    ${PARENT_DIR}/MechoSoma/XTool/xerrhand.cpp
    ${PARENT_DIR}/MechoSoma/XTool/xstream.cpp
)

include_directories(${PARENT_DIR}/MechoSoma/ParserC)
include_directories(${PARENT_DIR}/MechoSoma/XTool)
include_directories(${PARENT_DIR}/MechoSoma/iScreen)

add_executable(${APPLICATION_NAME} ${SOURCES})
target_link_libraries(${APPLICATION_NAME} PRIVATE ZLIB::ZLIB Iconv::Iconv)

set_target_properties(${APPLICATION_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
