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

if(WIN32)
  find_package(D3D12)
  
  sequoia_export_package(
    NAME D3D12
    FOUND ${D3D12_FOUND} 
    LIBRARIES ${D3D12_LIBRARIES}
    VERSION ${D3D12_VERSION}
    INCLUDE_DIRS ${D3D12_INCLUDE_DIRS}
  )
  
  if(D3D12_FOUND)
    add_definitions(-DSEQUOIA_HAS_D3D12)
  endif()

  set(has_d3d12 ON)
else()
  set(has_d3d12 OFF)
endif()

set(SEQUOIA_HAS_D3D12 ${has_d3d12} CACHE INTERNAL "Direct3D 12 is available" FORCE)
