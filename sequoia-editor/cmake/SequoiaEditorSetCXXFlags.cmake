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

include(SequoiaCheckAndSetCXXFlag)
include(SequoiaEnableClangMSan)
include(SequoiaEnableClangASan)

# sequoia_editor_set_cxx_flags
# ----------------------------
#
# Most of the underlying flags are inherited from the engine -- this only sets editor specific 
# flags.
#
macro(sequoia_editor_set_cxx_flags)
  # Add definitions to build shared libs
  if(BUILD_SHARED_LIBS)
    add_definitions(-DSEQUOIA_EDITOR_SHARED_LIBRARIES)
  endif()

  if(NOT(SEQUOIA_EDITOR_ASSERTS))
    add_definitions(-DNDEBUG)
    add_definitions(-DSEQUOIA_DISABLE_ASSERTS)
  endif()

  #
  # MSVC
  #
  if(SEQUOIA_COMPILER_MSVC)

  #
  # GCC/Clang/Intel
  #
  else()

    # Always build position independent code
    sequoia_check_and_set_cxx_flag("-fPIC" HAVE_GCC_PIC)

  endif()
endmacro()
