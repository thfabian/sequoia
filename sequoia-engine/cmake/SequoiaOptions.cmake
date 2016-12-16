##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
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

##===---------------------------------- Testing -----------------------------------------------===##
option(SEQUOIA_TESTING "Enable testing" ON)
CMAKE_DEPENDENT_OPTION(SEQUOIA_CORE_TESTING "Enable core unittests" ON "SEQUOIA_TESTING" OFF)

