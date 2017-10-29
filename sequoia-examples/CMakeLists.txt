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

# Default build type: RelWithDebInfo
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING 
      "Choose the type of build, options are: Debug Release RelWithDebInfo." FORCE)
endif()

if(DEFINED CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})
endif()

# Default library mode: static
if(NOT BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries." FORCE)
endif()

project(SequoiaExamples C CXX)
cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Initialize sequoia-cmake
include(SequoiaCMakeInit)
sequoia_cmake_init()

include(SequoiaAddExecutable)
include(SequoiaAddLibrary)
include(SequoiaAddTargetClangFormat)
include(SequoiaAddTargetCleanAll)
include(SequoiaCheckCompiler)
include(SequoiaCheckInSourceBuild)
include(SequoiaCombineLibraries)
include(SequoiaConfigureFile)
include(SequoiaCreatePackageString)
include(SequoiaExportPackage)
include(SequoiaGetArchitectureInfo)
include(SequoiaGetCompilerInfo)
include(SequoiaGetGitHeadRevision)
include(SequoiaGetPlatformInfo)
include(SequoiaMakeStringPair)
include(SequoiaReportResult)
include(SequoiaSetCXXStandard)

sequoia_get_compiler_info()
sequoia_get_platform_info()
sequoia_get_architecture_info()

include(SequoiaExamplesOptions)
#include(SequoiaExamplesDefinitions)
#include(SequoiaExamplesSetCXXFlags)

# Prevent in source builds
sequoia_check_in_source_build()

# Check compliance of compiler
sequoia_check_compiler(
  GCC 5.0
  CLANG 3.4
  MSVC 19.0
  INTEL 17
)

# Set C++ standard
sequoia_set_cxx_standard(c++14)

# Set C++ flags
#sequoia_engine_set_cxx_flags()

# Custom tragets
sequoia_add_target_clean_all(
  ${CMAKE_BINARY_DIR}/bin
  ${CMAKE_BINARY_DIR}/cmake
  ${CMAKE_BINARY_DIR}/docs
  ${CMAKE_BINARY_DIR}/lib
  ${CMAKE_BINARY_DIR}/ressource
  ${CMAKE_BINARY_DIR}/sequoia-cmake
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_BINARY_DIR}/test
)
sequoia_add_target_clang_format(
  DIRECTORIES 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/test
  EXTENSION ".h;.cpp"
)

# Output summary of the configuration
macro(make_config_string FIRST SECOND)
  sequoia_make_string_pair(${FIRST} ": ${SECOND}" 25 out)
  list(APPEND config_info ${out})
endmacro()

make_config_string("Platform" ${SEQUOIA_PLATFORM_STRING})
make_config_string("Architecture" ${SEQUOIA_ARCHITECTURE_STRING})
make_config_string("Compiler" ${SEQUOIA_COMPILER_STRING})
make_config_string("Build type" ${CMAKE_BUILD_TYPE})
make_config_string("Build shared" ${BUILD_SHARED_LIBS})
make_config_string("Install directory" ${CMAKE_INSTALL_PREFIX})

sequoia_report_result("Configuration summary" ${config_info})

# # Find the packages
# macro(find_packages NAME)
#   foreach(package ${ARGN})
#     include("Add${package}")
#     string(TOUPPER ${package} PACKAGE)

#     list(APPEND SEQUOIA_ENGINE_${NAME}_LIBRARIES ${SEQUOIA_${PACKAGE}_LIBRARIES})
#     list(APPEND SEQUOIA_ENGINE_${NAME}_INCLUDE_DIRS ${SEQUOIA_${PACKAGE}_INCLUDE_DIRS})
#     list(APPEND SEQUOIA_ENGINE_${NAME}_DEFINITIONS ${SEQUOIA_${PACKAGE}_DEFINITIONS})
    
#     sequoia_create_package_string(${package} info)
#     list(APPEND package_info ${info})
#   endforeach()
  
#   include_directories(SYSTEM ${SEQUOIA_ENGINE_${NAME}_INCLUDE_DIRS})
#   add_definitions(${SEQUOIA_ENGINE_${NAME}_DEFINITIONS})
# endmacro()

# find_packages(EXTERNAL Backward Boost glbinding glfw3 gli glm OpenCV OpenGL Python3 TBB fmt 
#                        tinyobjloader Protobuf Threads ccache clang-format)
# if(SEQUOIA_ENGINE_TESTING)
#   find_packages(TESTING GTest benchmark)
# endif()

# # Output summary of the packages
# sequoia_report_result("Package summary" ${package_info})
  
# # Build sequoia-engine
# include_directories(${CMAKE_SOURCE_DIR}/src)
# include_directories(${CMAKE_BINARY_DIR}/src)

# add_subdirectory(src)

# # Testing
# if(SEQUOIA_ENGINE_TESTING)
#   enable_testing()
#   add_subdirectory(test)
# endif()

# # Documentation
# if(SEQUOIA_ENGINE_DOCUMENTATION)
#   add_subdirectory(docs)
# endif()
