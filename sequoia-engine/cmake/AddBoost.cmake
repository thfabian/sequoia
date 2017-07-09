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
  set(Boost_USE_STATIC_RUNTIME OFF)
  add_definitions(-DBOOST_ALL_DYN_LINK)
else()
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME ON)
endif()

# Disable auto-linking
add_definitions(-DBOOST_ALL_NO_LIB)

# FindBoost
set(SEQUOIA_BOOST_COMPONENTS filesystem system program_options)
find_package(Boost 1.58 COMPONENTS ${SEQUOIA_BOOST_COMPONENTS} REQUIRED)

# Add include directories
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Copy DLL's to bin directories on windows
if(WIN32)
  foreach(build_type Release Debug RelWithDebInfo)
    foreach(component ${SEQUOIA_BOOST_COMPONENTS})
      string(TOUPPER ${component} component_upper)
      if("${build_type}" MATCHES "Debug")
        set(boost_lib "${Boost_${component_upper}_LIBRARY_DEBUG}")
      else()
        set(boost_lib "${Boost_${component_upper}_LIBRARY_RELEASE}")
      endif()
      
      # Boost DLL's are in the same folder as the libraries
      get_filename_component(lib_dir "${boost_lib}" DIRECTORY )
      get_filename_component(lib_name "${boost_lib}" NAME_WE)
      file(COPY "${lib_dir}/${lib_name}.dll" 
           DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/${build_type}")
    endforeach()
  endforeach()
endif()

sequoia_export_package(
  PACKAGE Boost
  FOUND ${Boost_FOUND} 
  VERSION_STR "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}" 
  LIBRARIES ${Boost_LIBRARIES}
)
