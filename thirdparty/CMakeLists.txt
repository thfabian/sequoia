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

option(USE_SYSTEM_BACKWARD "Should we use the system backward?" OFF)
option(USE_SYSTEM_BOOST "Should we use the system Boost?" OFF)
option(USE_SYSTEM_GLBINDING "Should we use the system glbinding?" OFF)
option(USE_SYSTEM_GLFW "Should we use the system GLFW?" OFF)
option(USE_SYSTEM_GLM "Should we use the system glm?" OFF)
option(USE_SYSTEM_GLSLANG "Should we use the system glslang?" OFF)
option(USE_SYSTEM_GTEST "Should we use the system GTest?" OFF)
option(USE_SYSTEM_TBB "Should we use the system TBB?" OFF)

#
# backward
#
if(USE_SYSTEM_BACKWARD)
  find_package(backward REQUIRED)
  if(DEFINED BACKWARD_ROOT)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DBACKWARD_ROOT:PATH=${BACKWARD_ROOT}")
  endif()
else()
  include(External_backward)
endif()

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
# glbinding
#
if(USE_SYSTEM_GLBINDING)
  find_package(glbinding ${glbinding_version} REQUIRED)
  if(DEFINED GLBINDING_ROOT)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DGLBINDING_ROOT:PATH=${GLBINDING_ROOT}")
  endif()
else()
  include(External_glbinding)
endif()

#
# GLFW
#
if(USE_SYSTEM_GLFW)
  find_package(GLFW ${glfw_version} REQUIRED)
  if(DEFINED GLFW_ROOT)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DGLFW_ROOT:PATH=${GLFW_ROOT}")
  endif()
else()
  include(External_glfw)
endif()

#
# glm
#
if(USE_SYSTEM_GLM)
  find_package(glm ${glm_version} REQUIRED)
  if(DEFINED GLM_ROOT)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DGLM_ROOT:PATH=${GLM_ROOT}")
  endif()
else()
  include(External_glm)
endif()

#
# glslang
#
if(USE_SYSTEM_GLSLANG)
  find_package(GLSLangValidator)
  if(DEFINED GLSLANGVALIDATOR_EXECUTABLE)
    list(APPEND Sequoia_THIRDPARTYLIBS_ARGS 
         "-DGLSLANGVALIDATOR_EXECUTABLE:PATH=${GLSLANGVALIDATOR_EXECUTABLE}")
  endif()
else()
  include(External_glslang)
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

