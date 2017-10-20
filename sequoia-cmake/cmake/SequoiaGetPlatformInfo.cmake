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

include(SequoiaIncludeGuard)
sequoia_include_guard()

#.rst:
# sequoia_get_platform_info
# -------------------------
#
# Get the identification string of the platform.
#
# .. code-block:: cmake
#
#   sequoia_get_platform_info()
#
# The functions defines the following variable:
#
# ``SEQUOIA_PLATFORM_STRING``
#   String of the platform.
#
# and conditionally the following:
#
# ``SEQUOIA_ON_WIN32``
#   Set to 1 if the platform is Win32-ish
# ``SEQUOIA_ON_UNIX``
#   Set to 1 if the platform is Unix-ish
# ``SEQUOIA_ON_APPLE``
#   Set to 1 if the platform is Darwin
# ``SEQUOIA_ON_LINUX``
#   Set to 1 if the platform is Linux
#
macro(sequoia_get_platform_info)
  if(WIN32)
    set(SEQUOIA_ON_WIN32 1 CACHE INTERNAL "Platform is Win32-ish" FORCE)
    set(SEQUOIA_PLATFORM_STRING "Windows" CACHE INTERNAL "Platform-id string" FORCE)
  elseif(APPLE)
    set(SEQUOIA_ON_UNIX 1 CACHE INTERNAL "Platform is Unix-ish" FORCE)
    set(SEQUOIA_ON_APPLE 1 CACHE INTERNAL "Platform is Darwin" FORCE)
    set(SEQUOIA_PLATFORM_STRING "Darwin" CACHE INTERNAL "Platform-id string" FORCE)
  elseif(UNIX)
    set(SEQUOIA_ON_UNIX 1 CACHE INTERNAL "Platform is Unix-ish" FORCE)
    set(SEQUOIA_ON_LINUX 1 CACHE INTERNAL "Platform is Linux" FORCE)
    set(SEQUOIA_PLATFORM_STRING "Linux" CACHE INTERNAL "Platform-id string" FORCE)
  endif()
endmacro()
