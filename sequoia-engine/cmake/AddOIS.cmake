##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

find_package(OIS REQUIRED)
include_directories(SYSTEM ${OIS_INCLUDE_DIRS})

# Copy DLL's to bin directories on windows
if(WIN32)
  if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
    file(COPY "${OIS_BINARY_DBG}" 
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
  else()
    file(COPY "${OIS_BINARY_REL}" 
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
  endif()
endif()

sequoia_export_package_variable(
  OIS 
  ${OIS_FOUND} 
  "OIS: Found" 
  ${OIS_LIBRARIES}
)

