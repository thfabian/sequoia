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
# sequoia_export_options
# ----------------------
#
# Export a list of options in the variable ``<NAME>_OPTIONS`` which can be accessed by meta
# projects.
# 
# .. code-block:: cmake
#
#   sequoia_export_options(NAME ARGN)
#
# ``NAME``
#   Prefix of the options list.
# ``ARGN``
#   List of option names to export.
#
macro(sequoia_export_options NAME)
  set("${NAME}_OPTIONS" "" CACHE INTERNAL "Options of ${NAME}" FORCE)
  foreach(arg ${ARGN})
    list(APPEND "${NAME}_OPTIONS" "${arg}")
  endforeach()
endmacro()
