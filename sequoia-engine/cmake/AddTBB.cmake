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
include_directories(SYSTEM ${TBB_INCLUDE_DIRS})

set(tbb_libs)
set(tbb_dlls)

# Get the idivudual components 
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(${TBB_DEFINITIONS_DEBUG})
  foreach(comp ${components})
    if(TBB_${comp}_LIBRARY_DEBUG)  
      set(tbb_libs "${TBB_${comp}_LIBRARY_DEBUG}")
    endif()
    if(TBB_${comp}_BINARY_DEBUG)  
      set(tbb_dlls "${TBB_${comp}_BINARY_DEBUG}")
    endif()
  endforeach()
else()
  add_definitions(${TBB_DEFINITIONS_RELEASE})
  foreach(comp ${components})
    if(TBB_${comp}_LIBRARY_RELEASE)  
      set(tbb_libs "${TBB_${comp}_LIBRARY_RELEASE}")
    endif()
    if(TBB_${comp}_BINARY_RELEASE)  
      set(tbb_dlls "${TBB_${comp}_BINARY_RELEASE}")
    endif()
  endforeach()
endif()

if(NOT(tbb_libs))
  set(tbb_libs ${TBB_LIBRARIRES})
endif()

if(NOT(tbb_dlls))
  set(tbb_dlls ${TBB_BINARIES})
endif()

if(SEQUOIA_ON_LINUX)
  set(LIBDL_NO_HEADERS 1)
  find_package(LibDL REQUIRED)
  list(APPEND tbb_libs ${LIBDL_LIBRARIES})
endif()

# Copy DLL's to bin directories on windows
if(WIN32)
  foreach(build_type Release Debug RelWithDebInfo)
    foreach(dll_path ${tbb_dlls})
      file(COPY "${dll_path}"  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/${build_type}")
    endforeach()
  endforeach()
endif()

sequoia_export_package(
  PACKAGE TBB
  FOUND ${TBB_FOUND} 
  VERSION_STR "${TBB_VERSION}"
  LIBRARIES ${tbb_libs}
)

