# Writes build_info.h, the build's identification for the log and crash reports. It runs at
# every build (the build_info target), not when CMake configures: a value taken then goes
# stale as soon as a commit lands without CMake running again.
#
# cmake -DSOURCE_DIR=<repository> -DOUTPUT=<header> [-DBRANCH=...] [-DCOMMIT=...] -P build_info.cmake
#
# BRANCH and COMMIT, when given (-DGIT_BRANCH and -DGIT_COMMIT_HASH at configure), are used
# as they are.

if(NOT BRANCH)
    # A CI build: the tag (a Steam release, v1.0.40) or the branch it was made for.
    if(DEFINED ENV{GITHUB_REF_NAME} AND NOT "$ENV{GITHUB_REF_NAME}" STREQUAL "")
        set(BRANCH "$ENV{GITHUB_REF_NAME}")
    else()
        execute_process(
            COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY "${SOURCE_DIR}"
            OUTPUT_VARIABLE BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET)
        if(BRANCH STREQUAL "HEAD")
            set(BRANCH "detached")  # a commit checked out by itself, as a submodule is
        endif()
    endif()
endif()

if(NOT COMMIT)
    # This repository's commit, also where it is a submodule (Samogonki-Steam): that is the
    # game's code, while GITHUB_SHA would name the outer repository's commit.
    execute_process(
        COMMAND git rev-parse --short=7 HEAD
        WORKING_DIRECTORY "${SOURCE_DIR}"
        OUTPUT_VARIABLE COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE COMMIT_RESULT
        ERROR_QUIET)
    if(COMMIT_RESULT EQUAL 0 AND COMMIT)
        # Changes to tracked files that aren't committed: the build isn't that commit.
        execute_process(
            COMMAND git status --porcelain --untracked-files=no --ignore-submodules=dirty
            WORKING_DIRECTORY "${SOURCE_DIR}"
            OUTPUT_VARIABLE CHANGES
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET)
        if(CHANGES)
            string(APPEND COMMIT "-dirty")
        endif()
    elseif(DEFINED ENV{GITHUB_SHA} AND NOT "$ENV{GITHUB_SHA}" STREQUAL "")
        string(SUBSTRING "$ENV{GITHUB_SHA}" 0 7 COMMIT)
    endif()
endif()

if(NOT BRANCH)
    set(BRANCH "unknown")
endif()
if(NOT COMMIT)
    set(COMMIT "unknown")
endif()

set(CONTENT "// Made by cmake/build_info.cmake at every build.\n#define GIT_BRANCH \"${BRANCH}\"\n#define GIT_COMMIT_HASH \"${COMMIT}\"\n")
# Written only when it changes, so that the files including it aren't compiled again.
if(EXISTS "${OUTPUT}")
    file(READ "${OUTPUT}" OLD_CONTENT)
endif()
if(NOT "${OLD_CONTENT}" STREQUAL "${CONTENT}")
    file(WRITE "${OUTPUT}" "${CONTENT}")
endif()
