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

# sequoia_compute_optional_dependency
# -----------------------------------
#
# Convenience macro for computing optional dependencies dependening on whether USE_SYSTEM_<PACKAGE> 
# is ON.
#
#    PACKAGE:STRING=<>      - Package to depend on
#    DEP_VAR:STRING=<>      - Output variable containing the resolved dependencies
#
macro(sequoia_compute_optional_dependency PACKAGE DEP_VAR)
  string(TOUPPER "${PACKAGE}" package_upper)
  if(NOT USE_SYSTEM_${package_upper})
    list(APPEND ${DEP_VAR} ${PACKAGE})
  endif()
endmacro()