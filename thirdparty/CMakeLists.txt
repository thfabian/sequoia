##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

option(USE_HLSL2GLSL "Should we use HLSL2GLSL in OGRE?" OFF)
option(USE_GLSL_OPTIMIZER "Should we use the GLSLOptimizer in OGRE?" OFF)

option(USE_SYSTEM_BOOST "Should we use the system Boost?" OFF)
option(USE_SYSTEM_GTEST "Should we use the system GTest?" OFF)
option(USE_SYSTEM_OGRE "Should we use the system OGRE?" OFF)
option(USE_SYSTEM_TBB "Should we use the system TBB?" OFF)
option(USE_SYSTEM_HLSL2GLSL "Should we use the system HLSL2GLSL?" OFF)
option(USE_SYSTEM_GLSL_OPTIMIZER "Should we use the system GLSLOptimizer?" OFF)

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

if(USE_SYSTEM_TBB)
  find_package(TBB REQUIRED)
else()
  include(External_tbb)
endif()

if(USE_GLSL_OPTIMIZER)
  if(USE_SYSTEM_GLSL_OPTIMIZER)
    find_package(GLSLOptimizer REQUIRED)
  else()
    include(External_glsl-optimizer)
  endif()
  list(APPEND ogre_dependencies glsl-optimizer)
endif()

if(USE_HLSL2GLSL)
  if(USE_SYSTEM_HLSL2GLSL)
    find_package(HLSL2GLSL REQUIRED)
  else()
    include(External_hlsl2glsl)
  endif()
  list(APPEND ogre_dependencies hlsl2glsl)
endif()


if(USE_SYSTEM_OGRE)
  find_package(OGRE REQUIRED)
  if(DEFINED OGRE_HOME)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS
         "-DOGRE_HOME:PATH=${OGRE_HOME}")
  endif()
else()
  include(External_ogredeps)
  include(External_ogre)
endif()

set(Sequoia_THIRDPARTYLIBS_ARGS "${Sequoia_THIRDPARTYLIBS_ARGS}" PARENT_SCOPE)

