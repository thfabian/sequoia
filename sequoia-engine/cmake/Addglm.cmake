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
include_directories(SYSTEM ${GLM_INCLUDE_DIRS})

if(NOT(WIN32))
  add_definitions(-DGLM_FORCE_CXX14)
endif()

sequoia_export_package_variable(
  glm
  ${GLM_FOUND} 
  "glm: ${GLM_VERSION}" 
)

