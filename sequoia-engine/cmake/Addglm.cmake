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

find_package(glm REQUIRED)

if(NOT(WIN32))
  list(APPEND glm_definitions -DGLM_FORCE_CXX14)
endif()

sequoia_export_package(
  NAME glm
  FOUND ${GLM_FOUND} 
  VERSION "${GLM_VERSION}"
  INCLUDE_DIRS ${GLM_INCLUDE_DIRS}
  DEFINITIONS ${glm_definitions}
)
