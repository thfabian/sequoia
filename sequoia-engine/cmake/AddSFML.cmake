##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

include_directories(SYSTEM ${SFML_INCLUDE_DIR})

sequoia_export_package_variable(
  SFML 
  ${SFML_FOUND} 
  "SFML: ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR}.${SFML_VERSION_PATCH}" 
  ${SFML_LIBRARIES}
)
