##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##
##
## Master CMake file of Sequoia-Engine.
##
##===------------------------------------------------------------------------------------------===##

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

# Output findings
sequoia_report_result(
  "Configuration summary" 
  "Sequoia version: ${SEQUOIA_VERSION_STRING}"
  "Platform: ${SEQUOIA_PLATFORM_STRING}"
  "Compiler: ${SEQUOIA_COMPILER_STRING}"
  "Build type: ${CMAKE_BUILD_TYPE}"
)

##===---------------------------------- Add external libraries --------------------------------===##
include(AddBoost)
include(AddSFML)
include(AddThreads)

# Output findings
sequoia_report_result(
  "External libraries summary" 
  "${SEQUOIA_THREADS_INFO_STR}"
  "${SEQUOIA_BOOST_INFO_STR}"
  "${SEQUOIA_SFML_INFO_STR}"
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

