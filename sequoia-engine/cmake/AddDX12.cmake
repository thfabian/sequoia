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
  find_package(DX12)

  sequoia_export_package(
    NAME DX12
    FOUND ${DX12_FOUND} 
    LIBRARIES ${DX12_LIBRARIES}
    VERSION ${DX12_VERSION}
    INCLUDE_DIRS ${DX12_INCLUDE_DIRS}
  )
  
  if(DX12_FOUND)
    add_definitions(-DSEQUOIA_HAS_DX12)
  endif()
endif()