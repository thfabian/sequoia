##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

find_package(OGRE)

include_directories(SYSTEM ${OGRE_INCLUDE_DIRS})

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  list(APPEND ogre_libraries ${OGRE_LIBRARY_DBG})
else()
  list(APPEND ogre_libraries ${OGRE_LIBRARY_REL})
endif()

sequoia_export_package_variable(
  OGRE 
  ${OGRE_FOUND} 
  "Ogre: ${OGRE_VERSION}" 
  ${ogre_libraries}
)

