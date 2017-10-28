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

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set(TBB_USE_DEBUG_BUILD 1)
else()
  set(TBB_USE_DEBUG_BUILD 0)
endif()

set(components tbb)
find_package(TBB COMPONENTS ${components} REQUIRED)

set(tbb_libs)
set(tbb_definitions)

# Get the idivudual components 
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  list(APPEND tbb_definitions ${TBB_DEFINITIONS_DEBUG})
  foreach(comp ${components})
    if(TBB_${comp}_LIBRARY_DEBUG)  
      set(tbb_libs "${TBB_${comp}_LIBRARY_DEBUG}")
    endif()
  endforeach()
else()
  list(APPEND tbb_definitions ${TBB_DEFINITIONS_RELEASE})
  foreach(comp ${components})
    if(TBB_${comp}_LIBRARY_RELEASE)  
      set(tbb_libs "${TBB_${comp}_LIBRARY_RELEASE}")
    endif()
  endforeach()
endif()

if(NOT(tbb_libs))
  set(tbb_libs ${TBB_LIBRARIRES})
endif()

if(SEQUOIA_ON_LINUX)
  set(LIBDL_NO_HEADERS 1)
  find_package(LibDL REQUIRED)
  list(APPEND tbb_libs ${LIBDL_LIBRARIES})
endif()

sequoia_export_package(
  NAME TBB
  FOUND ${TBB_FOUND} 
  VERSION "${TBB_VERSION}"
  LIBRARIES ${tbb_libs}
  INCLUDE_DIRS ${TBB_INCLUDE_DIRS}
  DEFINITIONS ${tbb_definitions}
)
