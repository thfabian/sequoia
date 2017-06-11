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

include(CMakeDependentOption)

##===---------------------------------- Building ----------------------------------------------===##
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
CMAKE_DEPENDENT_OPTION(SEQUOIA_ENABLE_RPATH 
                       "Enable rpath support on Linux and Mac" ON "BUILD_SHARED_LIBS" OFF)

set(BUILD_IS_RELEASE OFF)
set(BUILD_IS_NOT_RELEASE ON)
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  set(BUILD_IS_RELEASE ON)
  set(BUILD_IS_NOT_RELEASE OFF)
endif()

option(SEQUOIA_ASSERTS "Enable asserts" ${BUILD_IS_NOT_RELEASE})
option(SEQUOIA_OPTIMIZE "Enable all optimizations" ${BUILD_IS_RELEASE})

##===---------------------------------- Testing -----------------------------------------------===##
option(SEQUOIA_TESTING "Enable testing" ON)

##===---------------------------------- Documentation -----------------------------------------===##
option(SEQUOIA_DOCUMENTATION "Enable documentation" OFF)

