##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

set(SEQUOIA_GLOBAL_CONFIG_PATH config/sequoia-config.ini CACHE STRING "Sequia config path")
set(SEQUOIA_OGRE_CONFIG_PATH config/ogre-config.cfg CACHE STRING "Ogre config path" )
set(SEQUOIA_OGRE_PLUGIN_PATH config/ogre-plugin.cfg CACHE STRING "Ogre plugin path")

add_custom_target(
  SequoiaConfiguration ALL
  # Run in bin/
  COMMAND ${CMAKE_COMMAND} -E copy_directory 
          ${CMAKE_SOURCE_DIR}/bin/config 
          ${CMAKE_BINARY_DIR}/bin/config
  # Run in source
  COMMAND ${CMAKE_COMMAND} -E copy_directory 
          ${CMAKE_SOURCE_DIR}/bin/config 
          ${CMAKE_BINARY_DIR}/src/sequoia/Driver/config
  DEPENDS ${SEQUOIA_OGRE_CONFIG_PATH}
          ${SEQUOIA_OGRE_PLUGIN_PATH}
)

