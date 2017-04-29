##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
##                                       | |                    
##                                       |_| 
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

find_package(glbinding REQUIRED)
include_directories(SYSTEM ${GLBINDING_INCLUDE_DIRS})

#TODO: Copy DLL's to bin directories on windows

sequoia_export_package_variable(
  glbinding 
  ${GLBINDING_FOUND} 
  "glbinding: ${GLBINDING_VERSION}" 
  ${GLBINDING_LIBRARIES}
)
