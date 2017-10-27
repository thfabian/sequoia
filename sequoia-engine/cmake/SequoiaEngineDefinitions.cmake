##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _
##                        / ____|                      (_)
##                       | (___   ___  __ _ _   _  ___  _  __ _
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
##                                       | |
##                                       |_|
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

# Version
if(NOT DEFINED SEQUOIA_ENGINE_VERSION_MAJOR)
  set(SEQUOIA_ENGINE_VERSION_MAJOR 0 CACHE INTERNAL "Major version of Sequoia" FORCE)
endif()

if(NOT DEFINED SEQUOIA_ENGINE_VERSION_MINOR)
  set(SEQUOIA_ENGINE_VERSION_MINOR 0 CACHE INTERNAL "Minor version of Sequoia" FORCE)
endif()

if(NOT DEFINED SEQUOIA_ENGINE_VERSION_PATCH)
  set(SEQUOIA_ENGINE_VERSION_PATCH 1 CACHE INTERNAL "Patch version of Sequoia" FORCE)
endif()

if(NOT DEFINED SEQUOIA_ENGINE_VERSION_SUFFIX)
  set(SEQUOIA_ENGINE_VERSION_SUFFIX "dev" CACHE INTERNAL "Suffix of the Sequoia version" FORCE)
endif()

set(SEQUOIA_VERSION 
    ${SEQUOIA_ENGINE_VERSION_MAJOR}.${SEQUOIA_ENGINE_VERSION_MINOR}.${SEQUOIA_ENGINE_VERSION_PATCH}
    CACHE STRING "Version of Sequoia" FORCE)
mark_as_advanced(SEQUOIA_VERSION)

# Git version string
sequoia_get_git_head_revision(git_refspec git_hash)
if(NOT DEFINED SSEQUOIA_ENGINE_GIT_HASH OR NOT "${SSEQUOIA_ENGINE_GIT_HASH}" STREQUAL "${git_hash}")
  string(SUBSTRING ${git_hash} 0 7 git_hash_short)
  set(SSEQUOIA_ENGINE_GIT_HASH "${git_hash_short}" 
      CACHE INTERNAL "git hash (short) of current head" FORCE)
endif()

# Assemble full version string
string(TOLOWER ${SEQUOIA_ARCHITECTURE_STRING} architecture)
string(TOLOWER ${SEQUOIA_PLATFORM_STRING} platform)
string(TOLOWER ${CMAKE_CXX_COMPILER_ID} compiler)
set(compiler "${compiler}-${CMAKE_CXX_COMPILER_VERSION}")
set(SEQUOIA_ENGINE_FULL_VERSION 
    "${SEQUOIA_VERSION}-${SSEQUOIA_ENGINE_GIT_HASH}-${architecture}-${platform}-${compiler}"
    CACHE STRING "Full version string of Sequoia" FORCE)
mark_as_advanced(SEQUOIA_ENGINE_FULL_VERSION)

# Installation definitions
set(SEQUOIA_ENGINE_INSTALL_BIN_DIR bin 
    CACHE INTERNAL "Relative path of the binary install location" FORCE)
set(SEQUOIA_ENGINE_INSTALL_INCLUDE_DIR include 
    CACHE INTERNAL "Relative path of the include install location" FORCE)
set(SEQUOIA_ENGINE_INSTALL_LIB_DIR lib 
    CACHE INTERNAL "Relative path of the library install location " FORCE)
set(SEQUOIA_ENGINE_INSTALL_CMAKE_DIR cmake 
    CACHE INTERNAL "Relative path of the cmake install location" FORCE)

# Config.h paths
set(SEQUOIA_ENGINE_RESSOURCEPATH ${CMAKE_SOURCE_DIR}/ressource)

set(SEQUOIA_ENGINE_UNITTEST_RESSOURCEPATH ${CMAKE_SOURCE_DIR}/test/ressource)
set(SEQUOIA_ENGINE_UNITTEST_TEMPORARYPATH ${CMAKE_BINARY_DIR}/test/bin)

