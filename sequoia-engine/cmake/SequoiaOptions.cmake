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

set(BUILD_IS_RELEASE OFF)
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  set(BUILD_IS_RELEASE ON)
endif()
option(SEQUOIA_ASSERTS "Enable asserts" ${BUILD_IS_RELEASE})

##===---------------------------------- Testing -----------------------------------------------===##
option(SEQUOIA_TESTING "Enable testing" ON)
CMAKE_DEPENDENT_OPTION(SEQUOIA_CORE_TESTING "Enable core unittests" ON "SEQUOIA_TESTING" OFF)

##===---------------------------------- Documentation -----------------------------------------===##
option(SEQUOIA_DOCUMENTATION "Enable documentation" ON)

##===---------------------------------- Media files -------------------------------------------===##
set(SEQUOIA_MEDIA_DIR "${CMAKE_SOURCE_DIR}/../sequoia-media"
    CACHE PATH "Directory containing the media files")
