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
option(SEQUOIA_EXAMPLES_ASSERTS "Enable asserts" ${build_is_not_release})
option(SEQUOIA_EXAMPLES_OPTIMIZE "Enable all optimizations" ${build_is_release})
option(SEQUOIA_EXAMPLES_USE_CCACHE "Use compile cache if available" ON)

sequoia_export_options(SEQUOIA_EXAMPLES
  SEQUOIA_EXAMPLES_ASSERTS
  SEQUOIA_EXAMPLES_OPTIMIZE
  SEQUOIA_EXAMPLES_USE_CCACHE
)