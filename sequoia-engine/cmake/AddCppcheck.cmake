##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
##                                       | |                    
##                                       |_| 
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

find_package(Cppcheck)

if(CPPCHECK_FOUND)
  set(SEQUOIA_HAS_CPPCHECK 1)
  sequoia_export_package_variable(
    Cppcheck 
    ${CPPCHECK_FOUND} 
    "Cppcheck: Found" 
  )
else()
  set(SEQUOIA_HAS_CPPCHECK 0)
endif()

