##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

if(WIN32)
 add_custom_target(
     SequoiaConfiguration ALL
     # Run in bin/Release
     COMMAND ${CMAKE_COMMAND} -E copy_directory 
             ${CMAKE_SOURCE_DIR}/bin/config 
             ${CMAKE_BINARY_DIR}/bin/Release/config
     # Run in bin/Debug 
     COMMAND ${CMAKE_COMMAND} -E copy_directory 
             ${CMAKE_BINARY_DIR}/bin/config 
             ${CMAKE_BINARY_DIR}/bin/Debug/config
     # Run in bin/RelWithDebInfo
     COMMAND ${CMAKE_COMMAND} -E copy_directory 
             ${CMAKE_BINARY_DIR}/bin/config 
             ${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/config
     # Run in source
     COMMAND ${CMAKE_COMMAND} -E copy_directory 
             ${CMAKE_BINARY_DIR}/bin/config 
             ${CMAKE_BINARY_DIR}/src/sequoia/Driver/config

     DEPENDS ${CMAKE_BINARY_DIR}/bin/${SEQUOIA_OGRE_CONFIG_PATH}
             ${CMAKE_BINARY_DIR}/bin/${SEQUOIA_OGRE_CONFIG_PATH}
  )
else()
  add_custom_target(
     SequoiaConfiguration ALL
     # Run in source
     COMMAND ${CMAKE_COMMAND} -E copy_directory 
             ${CMAKE_BINARY_DIR}/bin/config 
             ${CMAKE_BINARY_DIR}/src/sequoia/Driver/config

     DEPENDS ${CMAKE_BINARY_DIR}/bin/${SEQUOIA_OGRE_CONFIG_PATH}
             ${CMAKE_BINARY_DIR}/bin/${SEQUOIA_OGRE_CONFIG_PATH}
 )
endif()

