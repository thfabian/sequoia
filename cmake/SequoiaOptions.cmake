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

set(build_is_not_release ON)
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  set(build_is_not_release OFF)
endif()

set(no_system_libs OFF)
if(WIN32)
  set(no_system_libs ON)
endif()

# Subprojects
set(SEQUOIA_ENGINE_DIR "${CMAKE_SOURCE_DIR}/sequoia-engine" CACHE PATH "Directory of the Engine")
set(SEQUOIA_DOCS_DIR "${CMAKE_SOURCE_DIR}/sequoia-docs" CACHE PATH "Directory of the Docs")

# Shared options
option(SEQUOIA_ASSERTS "Enable asserts in all sequoia projects" ${build_is_not_release})
option(SEQUOIA_NO_SYSTEM_LIBS "Don't use system libraries" ${no_system_libs})
