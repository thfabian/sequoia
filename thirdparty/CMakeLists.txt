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

option(USE_SYSTEM_BOOST "Should we use the system Boost?" OFF)
option(USE_SYSTEM_GTEST "Should we use the system GTest?" OFF)
option(USE_SYSTEM_BLAZE "Should we use the system Blaze?" OFF)
option(USE_SYSTEM_TBB "Should we use the system TBB?" OFF)

#
# Boost
#
if(USE_SYSTEM_BOOST)
  find_package(Boost ${boost_min_version} COMPONENTS ${boost_components} REQUIRED)

  if(DEFINED BOOST_ROOT)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DBOOST_ROOT:PATH=${BOOST_ROOT}")
  endif()
  list(APPEND Sequoia_THIRDPARTYLIBS_ARGS
    "-DBOOST_LIBRARYDIR:PATH=${Boost_LIBRARY_DIRS}"
    "-DBOOST_INCLUDEDIR:PATH=${Boost_INCLUDE_DIRS}")
else()
  include(External_boost)
endif()

#
# Blaze
#
if(USE_SYSTEM_BLAZE)
  find_package(Blaze ${blaze_version} REQUIRED)
  if(DEFINED BLAZE_ROOT)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DBLAZE_ROOT:PATH=${BLAZE_ROOT}")
  endif()
else()
  include(External_blaze)
endif()

#
# GTest
#
if(USE_SYSTEM_GTEST)
  find_package(GTest REQUIRED)
  if(DEFINED GTEST_ROOT)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DGTEST_ROOT:PATH=${GTEST_ROOT}")
  endif()
else()
  include(External_gtest)
endif()

#
# TBB
#
if(USE_SYSTEM_TBB)
  find_package(TBB REQUIRED)
  if(DEFINED TBB_ROOT_DIR)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DTBB_ROOT_DIR:PATH=${TBB_ROOT_DIR}")
  endif()
else()
  include(External_tbb)
endif()

set(Sequoia_THIRDPARTYLIBS_ARGS "${Sequoia_THIRDPARTYLIBS_ARGS}" PARENT_SCOPE)

