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

if(WIN32)
  set(NO_SYSTEM_LIBS_DEFAULT ON)
else()
  set(NO_SYSTEM_LIBS_DEFAULT OFF)
endif()

option(NO_SYSTEM_LIBS "By default, do not use system libraries" ${NO_SYSTEM_LIBS_DEFAULT})

set(Sequoia_THIRDPARTY_CMAKE_ARGS)
set(Sequoia_THIRDPARTY_PREFIX_PATH ${Sequoia_PREFIX_PATH})

#
# Packages built by default
#
sequoia_build_package(backward)
sequoia_build_package(benchmark)
sequoia_build_package(glfw3)
sequoia_build_package(gli)
sequoia_build_package(glm)
sequoia_build_package(gtest)

#
# Packages which are conditionally built if not found
#

# ZLIB
sequoia_find_package(
  PACKAGE ZLIB 
  FORWARD_VARS ZLIB_ROOT
)

# Boost
sequoia_find_package(
  PACKAGE Boost
  PACKAGE_ARGS ${boost_min_version} COMPONENTS ${boost_components}
  FORWARD_VARS BOOST_ROOT Boost_LIBRARY_DIRS Boost_INCLUDE_DIRS
)

# glbinding
sequoia_find_package(
  PACKAGE glbinding 
  PACKAGE_ARGS ${glbinding_version}
  FORWARD_VARS GLBINDING_ROOT
)

# OpenCV
sequoia_find_package(
  PACKAGE OpenCV
  PACKAGE_ARGS NO_MODULE QUIET
  REQUIRED_VARS OPENCV_CORE_FOUND OPENCV_HIGHGUI_FOUND OPENCV_IMGCODECS_FOUND OPENCV_IMGPROC_FOUND
  FORWARD_VARS OpenCV_DIR
  DEPENDS zlib
)

# TBB
sequoia_find_package(
  PACKAGE TBB 
  FORWARD_VARS TBB_ROOT_DIR
)

# Prepare and export CMake variables of the external projects
set(thirdparty_cmake_args
  -DCMAKE_PREFIX_PATH:STRING=${Sequoia_THIRDPARTY_PREFIX_PATH}
  ${Sequoia_CMAKE_ARGS}
  ${Sequoia_THIRDPARTY_CMAKE_ARGS}
)
set(Sequoia_CMAKE_ARGS "${thirdparty_cmake_args}" PARENT_SCOPE)

include(SequoiaReportResult)
sequoia_report_result("Package summary" ${SEQUOIA_PACKAGE_INFO})