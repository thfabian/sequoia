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

set(DOC_DIR ${PROJECT_SOURCE_DIR}/docs)

## Find doxygen
find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
  message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif()

set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile)
configure_file(${doxyfile_in} ${doxyfile} @ONLY)

add_custom_target(doxygen
  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating doxygen documentation" VERBATIM
)

