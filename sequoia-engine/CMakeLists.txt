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
cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Initialize sequoia-cmake
include(SequoiaCMakeInit)
sequoia_cmake_init()

include(SequoiaAddTargetCleanAll)
include(SequoiaCheckInSourceBuild)
include(SequoiaCreatePackageString)
include(SequoiaExportPackage)
include(SequoiaMakeStringPair)
include(SequoiaReportResult)

include(SequoiaEngineDefinitions)
include(SequoiaEngineFunctions)
include(SequoiaEngineMacros)
include(SequoiaEngineOptions)

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

# Output summary of the configuration
macro(make_config_string FIRST SECOND)
  sequoia_make_string_pair(${FIRST} ": ${SECOND}" 20 out)
  list(APPEND config_info ${out})
endmacro()
make_config_string("Sequoia version" ${SEQUOIA_VERSION_STRING})
make_config_string("Platform" ${SEQUOIA_PLATFORM_STRING})
make_config_string("Architecture" ${SEQUOIA_ARCHITECTURE_STRING})
make_config_string("Compiler" ${SEQUOIA_COMPILER_STRING})
make_config_string("Build type" ${CMAKE_BUILD_TYPE})
make_config_string("Build shared" ${BUILD_SHARED_LIBS})
sequoia_report_result("Configuration summary" ${config_info})

# Find the packages
if(SEQUOIA_TESTING)
  set(testing_packages GTest benchmark)
endif()

foreach(package Backward Boost ccache Cppcheck glbinding glfw3 gli glm OpenCV OpenGL Python3 TBB
                tinyobjloader ${testing_packages} Threads)
  include("Add${package}")
  string(TOUPPER ${package} PACKAGE)
  list(APPEND SEQUOIA_EXTERNAL_LIBRARIES ${SEQUOIA_${PACKAGE}_LIBRARIES})
  list(APPEND SEQUOIA_EXTERNAL_INCLUDE_DIRS ${SEQUOIA_${PACKAGE}_INCLUDE_DIRS})
  list(APPEND SEQUOIA_EXTERNAL_DEFINITIONS ${SEQUOIA_${PACKAGE}_DEFINITIONS})
  
  sequoia_create_package_string(${package} info)
  list(APPEND package_info ${info})
endforeach()

include_directories(SYSTEM ${SEQUOIA_EXTERNAL_INCLUDE_DIRS})
add_definitions(${SEQUOIA_EXTERNAL_DEFINITIONS})

# Output summary of the packages
sequoia_report_result("Package summary" ${package_info})
  
# Sequoia libraries
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/src)

add_subdirectory(src)
add_subdirectory(ressource)

# Testing
if(SEQUOIA_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

# Documentation
if(SEQUOIA_DOCUMENTATION)
  add_subdirectory(docs)
endif()
