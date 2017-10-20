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
# If the variable ``VAR`` exists, ``VALUE`` is appended and the the new value of ``VAR`` is 
# exported to all scopes i.e cached internally.
#
# .. code-block:: cmake
#
#   sequoia_append_and_export_variable(VAR VALUE)
#
# ``VAR``
#   Name of the variable.
# ``VALUE``
#   Value to append or, if the variable does not exists, it's new value. 
#
macro(sequoia_append_and_export_variable VAR VALUE)
  unset("__tmp_${VAR}")
  if(DEFINED ${VAR})
    set("__tmp_${VAR}" ${${VAR}})
  endif()
  list(APPEND "__tmp_${VAR}" ${VALUE})
  sequoia_set_and_export_variable(${VAR} "${__tmp_${VAR}}")
endmacro()
