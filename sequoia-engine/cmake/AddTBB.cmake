##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

find_package(TBB REQUIRED)

include_directories(SYSTEM ${TBB_INCLUDE_DIRS})

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(${TBB_DEFINITIONS_DEBUG})
else()
  add_definitions(${TBB_DEFINITIONS_RELEASE})
endif()

sequoia_export_package_variable(
  TBB 
  ${TBB_FOUND} 
  "TBB: ${TBB_VERSION}" 
  ${TBB_LIBRARIES}
)
