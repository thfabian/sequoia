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

# zlib
sequoia_find_package(
  PACKAGE ZLIB 
  FORWARD_VARS ZLIB_ROOT
)

# backward
sequoia_build_package(backward)

# Boost
sequoia_find_package(
  PACKAGE Boost
  PACKAGE_ARGS ${boost_min_version} COMPONENTS ${boost_components}
  FORWARD_VARS BOOST_ROOT Boost_LIBRARY_DIRS Boost_INCLUDE_DIRS
)

# FreeImage
sequoia_build_package(FreeImage)

# glbinding
sequoia_find_package(
  PACKAGE glbinding 
  PACKAGE_ARGS ${glbinding_version}
  FORWARD_VARS GLBINDING_ROOT
)

# glfw3
sequoia_find_package(
  PACKAGE GLFW 
  PACKAGE_ARGS ${glfw_version}
  FORWARD_VARS GLFW_ROOT
)

# gli
sequoia_build_package(gli)

# glm
sequoia_build_package(glm)

# gtest
sequoia_build_package(gtest)

# OpenCV
sequoia_find_package(
  PACKAGE OpenCV 
  PACKAGE_ARGS NO_MODULE QUIET
)

# tbb
sequoia_find_package(
  PACKAGE TBB 
  FORWARD_VARS TBB_ROOT_DIR
)

set(Sequoia_THIRDPARTYLIBS_ARGS "${Sequoia_THIRDPARTYLIBS_ARGS}" PARENT_SCOPE)
sequoia_report_result("Package summary" ${SEQUOIA_PACKAGE_INFO})

