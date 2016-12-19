##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

option(USE_SYSTEM_BOOST "Should we use the system Boost?" OFF)
option(USE_SYSTEM_GTEST "Should we use the system GTest?" OFF)
option(USE_SYSTEM_SFML "Should we use the system SFML?" OFF)
option(USE_SYSTEM_TBB "Should we use the system TBB?" OFF)

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

if(USE_SYSTEM_GTEST)
  find_package(GTest REQUIRED)
else()
  include(External_gtest)
endif()

if(USE_SYSTEM_SFML)
  find_package(SFML ${sfml_version} COMPONENTS ${sfml_components} REQUIRED)
else()
  include(External_sfml)
endif()

if(USE_SYSTEM_TBB)
  find_package(TBB REQUIRED)
else()
  include(External_tbb)
endif()

set(Sequoia_THIRDPARTYLIBS_ARGS "${Sequoia_THIRDPARTYLIBS_ARGS}" PARENT_SCOPE)

