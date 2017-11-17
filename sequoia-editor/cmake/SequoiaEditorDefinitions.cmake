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
include(SequoiaEditorVersion)

# Git version string
sequoia_get_git_head_revision(git_refspec git_hash)
string(SUBSTRING ${git_hash} 0 7 git_hash_short)
if(NOT DEFINED SEQUOIA_EDITOR_GIT_HASH OR NOT "${SEQUOIA_EDITOR_GIT_HASH}" STREQUAL "${git_hash_short}")
  set(SEQUOIA_EDITOR_GIT_HASH "${git_hash_short}" 
      CACHE INTERNAL "git hash (short) of current head" FORCE)
endif()

# Assemble full version string
string(TOLOWER ${SEQUOIA_ARCHITECTURE_STRING} architecture)
string(TOLOWER ${SEQUOIA_PLATFORM_STRING} platform)
string(TOLOWER ${CMAKE_CXX_COMPILER_ID} compiler)
set(compiler "${compiler}-${CMAKE_CXX_COMPILER_VERSION}")
set(SEQUOIA_EDITOR_FULL_VERSION 
    "${SEQUOIA_EDITOR_VERSION}-${SEQUOIA_EDITOR_GIT_HASH}-${architecture}-${platform}-${compiler}"
    CACHE STRING "Full version string of Sequoia" FORCE)
mark_as_advanced(SEQUOIA_EDITOR_FULL_VERSION)

# Installation definitions
set(SEQUOIA_EDITOR_INSTALL_BIN_DIR bin 
    CACHE INTERNAL "Relative path of the binary install location" FORCE)
set(SEQUOIA_EDITOR_INSTALL_INCLUDE_DIR include 
    CACHE INTERNAL "Relative path of the include install location" FORCE)
set(SEQUOIA_EDITOR_INSTALL_LIB_DIR lib 
    CACHE INTERNAL "Relative path of the library install location " FORCE)
set(SEQUOIA_EDITOR_INSTALL_CMAKE_DIR cmake/sequoia-engine
    CACHE INTERNAL "Relative path of the cmake install location" FORCE)

