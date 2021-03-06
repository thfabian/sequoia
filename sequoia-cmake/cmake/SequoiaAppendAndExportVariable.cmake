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

include(SequoiaSetAndExportVariable)

#.rst:
# sequoia_append_and_export_variable
# ----------------------------------
#
# If the variable ``VAR`` exists, ``ARGN`` is appended and to the new value of ``VAR`` and is 
# exported to all scopes i.e cached internally. Note that if the variable has duplicate values, 
# those are removed before assignment. 
#
# .. code-block:: cmake
#
#   sequoia_append_and_export_variable(VAR [ARGN...])
#
# ``VAR``
#   Name of the variable.
# ``ARGN``
#   Value(s) to append or, if the variable does not exists, it's new value(s). 
#
macro(sequoia_append_and_export_variable VAR)
  unset("__tmp_${VAR}")
  if(DEFINED ${VAR})
    set("__tmp_${VAR}" ${${VAR}})
  endif()
  list(APPEND "__tmp_${VAR}" ${ARGN})
  if(__tmp_${VAR})
    list(REMOVE_DUPLICATES "__tmp_${VAR}")
  endif()
  sequoia_set_and_export_variable(${VAR} "${__tmp_${VAR}}")
endmacro()
