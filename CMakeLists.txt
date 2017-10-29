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

# Default installation path: <build-dir>/install
if(NOT CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING
      "Install path prefix, prepended onto install directories." FORCE)
endif()

project(Sequoia C CXX)
cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Initialize sequoia-cmake
include(SequoiaCMakeInit)
sequoia_cmake_init()

include(SequoiaAddTargetCleanAll)
include(SequoiaAppendAndExportVariable)
include(SequoiaCheckInSourceBuild)
include(SequoiaGetArchitectureInfo)
include(SequoiaGetCompilerInfo)
include(SequoiaGetPlatformInfo)
include(SequoiaMakeStringPair)
include(SequoiaReportResult)
include(SequoiaSetAndExportVariable)

# Include all options (from all subprojects)
include(SequoiaOptions)

sequoia_get_compiler_info()
sequoia_get_platform_info()
sequoia_get_architecture_info()

# Prevent in source builds
sequoia_check_in_source_build()

# Add custom targets
sequoia_add_target_clean_all(
  "${CMAKE_BINARY_DIR}/prefix"  
  "${CMAKE_BINARY_DIR}/thirdparty"
  "${CMAKE_BINARY_DIR}/install"
)

# Output summary of the configuration
macro(make_config_string FIRST SECOND)
  sequoia_make_string_pair(${FIRST} ": ${SECOND}" 20 out)
  list(APPEND config_info ${out})
endmacro()

make_config_string("Platform" ${SEQUOIA_PLATFORM_STRING})
make_config_string("Architecture" ${SEQUOIA_ARCHITECTURE_STRING})
make_config_string("Compiler" ${SEQUOIA_COMPILER_STRING})
make_config_string("Build type" ${CMAKE_BUILD_TYPE})
make_config_string("Install directory" ${CMAKE_INSTALL_PREFIX})

sequoia_report_result("Configuration summary" ${config_info})

# Build all dependencies
set(SEQUOIA_EXTERNAL_CMAKE_ARGS 
  "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}"
  "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
  "-DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}"
  "-DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}"
  "-DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}"
)
set(SEQUOIA_EXTERNAL_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/prefix")
set(SEQUOIA_EXTERNAL_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

include(SequoiaSetDownloadDir)
sequoia_set_download_dir(SEQUOIA_EXTERNAL_DOWNLOAD_DIR)

if(SEQUOIA_ASSERTS)
  include(SequoiaSetAssertLevel)
  sequoia_set_assert_level(SEQUOIA_EXTERNAL_CMAKE_ARGS)
endif()

add_subdirectory(thirdparty)

# Build the sequoia projects
include(External_SequoiaEngine)
include(External_SequoiaExamples)
