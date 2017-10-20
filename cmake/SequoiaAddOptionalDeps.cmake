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

# sequoia_add_optional_deps
# -----------------------------
#
# Convenience macro for adding dependencies optionally if not using system libraries. This function 
# takes a list of external projects targets ARGN, looks for a variable of the form 
# USE_SYSTEM_<ARG> for each ARG in ARGN, if this does not exist or is set to FALSE the supplied 
# taget name will be added to DEP_VAR as a dependency.
#
#    DEP_VAR:STRING=<>      - Output variable containing the resolved dependencies
#    ARGN                   - Dependencies to append
#
macro(sequoia_add_optional_deps DEP_VAR)
  foreach(dep ${ARGN})
    string(TOUPPER "${dep}" dependency)
    if(NOT USE_SYSTEM_${dependency})
      list(APPEND ${DEP_VAR} ${dep})
    endif()
  endforeach()
endmacro()