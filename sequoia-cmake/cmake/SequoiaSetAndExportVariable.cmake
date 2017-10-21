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
# sequoia_set_and_export_variable
# -------------------------------
#
# Set the variable ``VAR`` to ``ARGN`` and export it to all scopes i.e cache it internally. 
#
# .. code-block:: cmake
#
#   sequoia_set_and_export_variable(VAR [ARGN...])
#
# ``VAR``
#   Name of the variable.
# ``ARGN``
#   New value(s) of the variable. 
#
macro(sequoia_set_and_export_variable VAR)
  set("${VAR}" "${ARGN}" CACHE INTERNAL "")
endmacro()
