##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

set(Boost_USE_MULTITHREADED ON)
if(BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS OFF)
  set(Boost_USE_STATIC_RUNTIME OFF)
else()
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME ON)
endif()

add_definitions(-DBOOST_LOG_DYN_LINK)
add_definitions(-DBOOST_ALL_DYN_LINK)

find_package(Boost 1.54 REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

sequoia_export_package_variable(
  BOOST 
  ${Boost_FOUND} 
  "Boost: ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}" 
  ${Boost_LIBRARIES}
)
