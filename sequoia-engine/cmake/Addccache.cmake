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

if(NOT(MSVC))
  find_package(ccache)

  if(CCACHE_FOUND)
    set(SEQUOIA_HAS_CCACHE 1)
    sequoia_export_package(
      PACKAGE ccache
      FOUND ${CCACHE_FOUND} 
    )

    if(SEQUOIA_USE_CCACHE)
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_EXECUTABLE})
      set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_EXECUTABLE})
    endif()

  else()
    set(SEQUOIA_HAS_CCACHE 0)
  endif()
endif()
