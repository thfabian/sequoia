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

option(NO_SYSTEM_LIBS "By default, do not use system libraries" OFF)

include(External_gtest)
include(External_stb)
include(External_backward)

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

# glbinding
sequoia_find_package(
  PACKAGE GLFW 
  PACKAGE_ARGS ${glfw_version}
  FORWARD_VARS GLFW_ROOT
)

# glm
sequoia_find_package(
  PACKAGE glm 
  PACKAGE_ARGS ${glm_version}
  FORWARD_VARS GLM_ROOT
)

# TBB
sequoia_find_package(
  PACKAGE TBB 
  FORWARD_VARS TBB_ROOT_DIR
)

set(Sequoia_THIRDPARTYLIBS_ARGS "${Sequoia_THIRDPARTYLIBS_ARGS}" PARENT_SCOPE)
sequoia_report_result("Package summary" ${SEQUOIA_PACKAGE_INFO})

