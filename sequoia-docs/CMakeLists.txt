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

project(SequoiaDocs)
cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Initialize sequoia-cmake
include(SequoiaCMakeInit)
sequoia_cmake_init()

# Prevent in source builds
include(SequoiaCheckInSourceBuild)
sequoia_check_in_source_build()

# Add clean-all target
include(SequoiaAddTargetCleanAll)
sequoia_add_target_clean_all(
  "${CMAKE_BINARY_DIR}/src" 
  "${CMAKE_BINARY_DIR}/sequoia-cmake"
)

# Include the packages
include(SequoiaCreatePackageString)
include(SequoiaExportPackage)
include(SequoiaReportResult)

foreach(package Python3 Sphinx Doxygen)
  include("Add${package}")
  sequoia_create_package_string(${package} info)
  list(APPEND package_info ${info})
endforeach()
sequoia_report_result("Package summary" ${package_info})

add_subdirectory(src)
