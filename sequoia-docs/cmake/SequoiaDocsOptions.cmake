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

include(SequoiaExportOptions)

set(build_is_not_release ON)
set(build_is_release OFF)
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  set(build_is_not_release OFF)
  set(build_is_release ON)
endif()

# sequoia-engine
set(SEQUOIA_DOCS_CMAKE_DIR "${SEQUOIA_CMAKE_DIR}" 
    CACHE PATH "Directory of the sequoia-cmake")

# sequoia-docs
set(SEQUOIA_DOCS_ENGINE_DIR "${CMAKE_SOURCE_DIR}/../sequoia-engine" 
    CACHE PATH "Directory of the sequoia-engine")

sequoia_export_options(SEQUOIA_DOCS
  SEQUOIA_DOCS_CMAKE_DIR
  SEQUOIA_DOCS_ENGINE_DIR
)