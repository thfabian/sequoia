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

##===---------------------------------- Building ----------------------------------------------===##
option(SEQUOIA_ENGINE_ASSERTS "Enable asserts" ${build_is_not_release})
option(SEQUOIA_ENGINE_OPTIMIZE "Enable all optimizations" ${build_is_release})
option(SEQUOIA_ENGINE_USE_CCACHE "Use compile cache if available" ON)

##===---------------------------------- Testing -----------------------------------------------===##
option(SEQUOIA_ENGINE_TESTING "Enable testing" ON)

##===---------------------------------- Documentation -----------------------------------------===##
option(SEQUOIA_ENGINE_DOCUMENTATION "Enable documentation" OFF)

sequoia_export_options(SEQUOIA_ENGINE
  SEQUOIA_ENGINE_ASSERTS
  SEQUOIA_ENGINE_OPTIMIZE
  SEQUOIA_ENGINE_USE_CCACHE
  SEQUOIA_ENGINE_TESTING
  SEQUOIA_ENGINE_DOCUMENTATION
)