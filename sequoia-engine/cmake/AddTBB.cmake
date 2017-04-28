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

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set(TBB_USE_DEBUG_BUILD 1)
else()
  set(TBB_USE_DEBUG_BUILD 0)
endif()

find_package(TBB REQUIRED)

set(tbb_libs ${TBB_LIBRARIES})
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(${TBB_DEFINITIONS_DEBUG})
  if(TBB_tbb_LIBRARY_DEBUG)  
    set(tbb_libs ${TBB_tbb_LIBRARY_DEBUG})
  endif()
else()
  add_definitions(${TBB_DEFINITIONS_RELEASE})
  if(TBB_tbb_LIBRARY_RELEASE)  
    set(tbb_libs ${TBB_tbb_LIBRARY_RELEASE})
  endif()
endif()

if(SEQUOIA_ON_LINUX)
  set(LIBDL_NO_HEADERS 1)
  find_package(LibDL REQUIRED)
  list(APPEND tbb_libs ${LIBDL_LIBRARIES})
endif()

sequoia_export_package_variable(
  TBB 
  ${TBB_FOUND} 
  "TBB: ${TBB_VERSION}"
  ${tbb_libs}
)

