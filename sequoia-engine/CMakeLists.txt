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

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING 
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

if(DEFINED CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})
endif()

project(SequoiaEngine C CXX)
cmake_minimum_required(VERSION 3.3)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../sequoia-cmake/cmake")

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
  "Build shared: ${BUILD_SHARED_LIBS}"
)

##===---------------------------------- Add external libraries --------------------------------===##

include(AddBackward)
include(AddBoost)
include(AddCppcheck)
include(Addccache)
include(Addglbinding)
include(AddGLFW)
include(Addgli)
include(Addglm)
include(AddOpenCV)
include(AddOpenGL)
include(AddPython3)
include(AddTBB)
include(AddThreads)

if(SEQUOIA_TESTING)
  include(AddGTest)
  include(Addbenchmark)
endif()

set(SEQUOIA_EXTERNAL_LIBRARIES
  ${SEQUOIA_BACKWARD_LIBRARIES}
  ${SEQUOIA_BOOST_LIBRARIES}
  ${SEQUOIA_GLFW_LIBRARIES}
  ${SEQUOIA_GLBINDING_LIBRARIES}
  ${SEQUOIA_OPENCV_LIBRARIES}
  ${SEQUOIA_OPENGL_LIBRARIES}
  ${SEQUOIA_TBB_LIBRARIES}
  ${SEQUOIA_THREADS_LIBRARIES}
  ${SEQUOIA_GTEST_LIBRARIES}
  ${SEQUOIA_BENCHMARK_TARGETS}
)

# Output summary of the packages
sequoia_report_result("External libraries summary" ${SEQUOIA_PACKAGE_INFO})
  
##===------------------------------------ Sequoia libraries -----------------------------------===##
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/src)

add_subdirectory(src)
add_subdirectory(ressource)

##===----------------------------------------- Testing ----------------------------------------===##
if(SEQUOIA_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

##===----------------------------------------- Documentation ----------------------------------===##
if(SEQUOIA_DOCUMENTATION)
  add_subdirectory(docs)
endif()

