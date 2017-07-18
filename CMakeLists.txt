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

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING 
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

if(DEFINED CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})
endif()

project(Sequoia C CXX)
cmake_minimum_required(VERSION 3.3)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/modules")

##===---------------------------------- CMake modules -----------------------------------------===##
include(SequoiaMacros)
include(ExternalProject)

# Prevent in source builds
sequoia_check_in_source_build()

# Set default download dir
sequoia_set_default_download_dir()

# Provide clean-all target which clear the CMakeCache
sequoia_add_target_clean_all()

# Set default CMake arguments
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(Sequoia_INSTALL_PREFIX "${Sequoia_BINARY_DIR}/prefix")
set(Sequoia_DEFAULT_ARGS
  "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}"
  "-DCMAKE_PREFIX_PATH:PATH=${Sequoia_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}"
  "-DCMAKE_INSTALL_PREFIX:PATH=${Sequoia_INSTALL_PREFIX}"
  "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
  "-DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}"
  "-DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}"
  "-DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}"
)

# Set assert level 
set(BUILD_IS_NOT_RELEASE ON)
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  set(BUILD_IS_NOT_RELEASE OFF)
endif()
option(SEQUOIA_ASSERTS "Enable asserts in all libraries i.e undefine -DNDEBUG" 
       ${BUILD_IS_NOT_RELEASE})
sequoia_set_assert_level()

##===---------------------------------- Third party projects ----------------------------------===##
include(Projects)
add_subdirectory(thirdparty)

##===---------------------------------- Sequoia projects --------------------------------------===##
include(External_sequoia-engine)

