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

set(Boost_USE_MULTITHREADED ON)
if(BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS OFF)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
  add_definitions(-DBOOST_ALL_DYN_LINK)
else()
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()

set(boost_components filesystem system program_options)
find_package(Boost 1.58 COMPONENTS ${boost_components} REQUIRED)

if(WIN32)
  # We filter the libraries manually as we want to export these libraries in the CMake install
  # config and ```Boost_LIBRARIES`` contains optimized/debug keywords.
  foreach(component ${boost_components})
    string(TOUPPER ${component} component_upper)
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      list(APPEND boost_libs ${Boost_${component_upper}_LIBRARY_DEBUG})
    else()
      list(APPEND boost_libs ${Boost_${component_upper}_LIBRARY_RELEASE})
    endif()
  endforeach()
else()
  set(boost_libs ${Boost_LIBRARIES})
endif()

sequoia_export_package(
  NAME Boost
  FOUND ${Boost_FOUND} 
  VERSION "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}" 
  LIBRARIES ${boost_libs}
  INCLUDE_DIRS ${Boost_INCLUDE_DIRS}
  DEFINITIONS -DBOOST_ALL_NO_LIB
)
