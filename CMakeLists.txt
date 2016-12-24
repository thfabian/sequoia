##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##
##
## Master CMake file of Sequoia framework.
##
##===------------------------------------------------------------------------------------------===##

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING 
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
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

# Set default CMake arguments
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(Sequoia_INSTALL_PREFIX "${Sequoia_BINARY_DIR}/prefix")
set(Sequoia_DEFAULT_ARGS
  "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}"
  "-DCMAKE_PREFIX_PATH:PATH=${Sequoia_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}"
  "-DCMAKE_INSTALL_PREFIX:PATH=${Sequoia_INSTALL_PREFIX}"
  "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
  "-DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}"
  "-DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}"
)

##===---------------------------------- Third party projects ----------------------------------===##
include(Projects)
add_subdirectory(thirdparty)

##===---------------------------------- Sequoia projects --------------------------------------===##
include(External_sequoia-engine)

if(MSVC)
  option(SETUP_SEQUOIA_BUILDS "Setup sequoia-engine builds" OFF)
else()
  option(SETUP_SEQUOIA_BUILDS "Setup sequoia-engine builds" ON)
endif()

if(SETUP_SEQUOIA_BUILDS)
  sequoia_setup_sequioa_engine_builds()
endif()

