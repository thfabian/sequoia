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

#.rst:
# FindDX12
# --------
#
# .. code-block:: cmake
#
#   find_package(DX12 
#     [REQUIRED]             # Fail with error if  Direct3D 12 SDK is not found
#     [QUIET]                # Supress output
#   )
#
# This module locates the Direct3D 12 SDK. ``DX12_FOUND`` will report if an acceptable version 
# of DX12 was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
# ``DX12_FOUND``
#   System has Direct3D 12 SDK.
# ``DX12_INCLUDE_DIRS``
#   The location of DX12 headers.
# ``DX12_LIBRARIES``
#   The location of DX12 libraries.
#
# Hints
# ^^^^^
#
# This script relies on the environment variables ``WindowsSdkDir`` and ``WindowsSDKVersion``.
#

include(FindPackageHandleStandardArgs)

if(WIN32)
  set(WIN10_SDK_DIR "$ENV{WindowsSdkDir}")
  string(REPLACE "\\\\" "" WIN10_SDK_VERSION "$ENV{WindowsSDKVersion}")
  set(DX12_VERSION ${WIN10_SDK_VERSION})

  #
  # Find DX12 headers
  #
  find_path(dxsdk_include_dir
    NAMES
      d3d12.h
    PATHS "${WIN10_SDK_DIR}/Include/${WIN10_SDK_VERSION}/um/"
  )

  foreach(header d3d12 d3dcompiler)
    find_path(DX12_${header}_INCLUDE_DIR
      NAMES 
        ${header}.h
      PATHS
        "${dxsdk_include_dir}"
    )
    list(APPEND DX12_INCLUDE_DIRS ${DX12_${header}_INCLUDE_DIR})
  endforeach()
  list(REMOVE_DUPLICATES DX12_INCLUDE_DIRS)

  #
  # Find DX12 libraries
  #
  find_library(dxsdk_library_dir
    NAMES
      d3d12.lib
    PATHS "${WIN10_SDK_DIR}/Lib/${WIN10_SDK_VERSION}/um/x64"
  )

  foreach(lib d3d12 d3dcompiler dxguid dxgi)
    find_library(DX12_${lib}_LIBRARY
      NAMES 
        ${lib}.lib
      PATHS
        "${dxsdk_library_dir}"
    )
    list(APPEND DX12_LIBRARIES ${DX12_${lib}_LIBRARY})
  endforeach()

  #
  # Report result 
  #
  find_package_handle_standard_args(
    DX12
    FAIL_MESSAGE "Could NOT find Direct3D 12 SDK"
    REQUIRED_VARS DX12_INCLUDE_DIRS DX12_LIBRARIES
    VERSION_VAR DX12_VERSION
  )

  mark_as_advanced(
    DX12_INCLUDE_DIRS
    DX12_LIBRARIES
  )

else()
  set(DX12_FOUND FALSE)
  set(DX12_INCLUDE_DIRS)
  set(DX12_LIBRARIES)
endif()

if(NOT(DX12_FOUND) AND DX12_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find Direct3D 12 SDK")
endif()
