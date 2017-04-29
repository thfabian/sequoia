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

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING 
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

project(SequoiaEngine C CXX)
cmake_minimum_required(VERSION 3.3)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/modules")

##===-------------------------------------- CMake modules -------------------------------------===##
include(SequoiaIncludeGuard)
include(SequoiaOptions)
include(SequoiaDefinitions)
include(SequoiaMacros)
include(SequoiaFunctions)

# Prevent in source builds
sequoia_check_in_source_build()

# Check compliance of compiler
sequoia_check_compiler()

# Set default build-type
sequoia_set_default_build_type(Release)

# Set C++ standard
sequoia_set_cxx_standard(c++14)

# Set C++ flags
sequoia_set_cxx_flags()

# Custom tragets
sequoia_add_target_clean_all()
sequoia_add_target_clang_format(${CMAKE_SOURCE_DIR}/src)

# Output findings
sequoia_report_result(
  "Configuration summary" 
  "Sequoia version: ${SEQUOIA_VERSION_STRING}"
  "Platform: ${SEQUOIA_PLATFORM_STRING}"
  "Architecture: ${SEQUOIA_ARCHITECTURE_STRING}"
  "Compiler: ${SEQUOIA_COMPILER_STRING}"
  "Build type: ${CMAKE_BUILD_TYPE}"
)

##===---------------------------------- Add external libraries --------------------------------===##
include(AddThreads)
include(AddBlaze)
include(AddBoost)
include(AddCppcheck)
include(Addglbinding)
include(AddGLFW)
include(AddGTest)
include(AddOpenGL)
include(AddTBB)

if(SEQUOIA_TESTING)
  include(AddGTest)
endif()

set(SEQUOIA_EXTERNAL_LIBRARIES
  ${SEQUOIA_GLFW_LIBRARIES}
  ${SEQUOIA_TBB_LIBRARIES}
  ${SEQUOIA_BOOST_LIBRARIES}
  ${SEQUOIA_GLBINDING_LIBRARIES}
  ${SEQUOIA_OPENGL_LIBRARIES}
  ${SEQUOIA_THREADS_LIBRARIES}
)

# Output findings
sequoia_report_result(
  "External libraries summary" 
  "${SEQUOIA_THREADS_INFO_STR}"
  "${SEQUOIA_BLAZE_INFO_STR}"
  "${SEQUOIA_BOOST_INFO_STR}"
  "${SEQUOIA_CPPCHECK_INFO_STR}"
  "${SEQUOIA_GLBINDING_INFO_STR}"
  "${SEQUOIA_GLFW_INFO_STR}"
  "${SEQUOIA_GTEST_INFO_STR}"
  "${SEQUOIA_OPENGL_INFO_STR}"
  "${SEQUOIA_TBB_INFO_STR}"
)
  
##===------------------------------------ Sequoia libraries -----------------------------------===##
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/src)

add_subdirectory(src)

##===----------------------------------------- Testing ----------------------------------------===##
if(SEQUOIA_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

##===----------------------------------------- Documentation ----------------------------------===##
if(SEQUOIA_DOCUMENTATION)
  add_subdirectory(docs)
endif()

