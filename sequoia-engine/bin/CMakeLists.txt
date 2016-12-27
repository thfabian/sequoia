##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

set(SEQUOIA_GLOBAL_CONFIG_PATH config/sequoia-config.json CACHE STRING "Sequia config path")
set(SEQUOIA_OGRE_CONFIG_PATH config/ogre-config.cfg CACHE STRING "Ogre config path" )
set(SEQUOIA_OGRE_PLUGIN_PATH config/ogre-plugin.cfg CACHE STRING "Ogre plugin path")

if(WIN32)
  add_custom_target(
    SequoiaConfiguration ALL
    # Run in bin/Release
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/bin/config 
            ${CMAKE_BINARY_DIR}/bin/Release/config
    # Run in bin/RelWithDebInfo
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/bin/config 
            ${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/config
    # Run in bin/Debug
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/bin/config 
            ${CMAKE_BINARY_DIR}/bin/Debug/config
    # Run in source
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/bin/config 
            ${CMAKE_BINARY_DIR}/src/sequoia/Driver/config
    DEPENDS ${SEQUOIA_OGRE_CONFIG_PATH}
            ${SEQUOIA_OGRE_PLUGIN_PATH}
  )
else()
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
endif()
