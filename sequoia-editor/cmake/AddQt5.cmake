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

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(qt5_modules Widgets Core Gui)
find_package(Qt5 5.9 REQUIRED COMPONENTS ${qt5_modules})

foreach(module ${qt5_modules})
  set(module_name "Qt5${module}")
  list(APPEND libraries ${${module_name}_LIBRARIES})
  list(APPEND include_dirs ${${module_name}_INCLUDE_DIRS})
  list(APPEND definitions ${${module_name}_DEFINITIONS})
endforeach()

# Find the windeployqt tool on windows to copy the DLLs
if(WIN32)
  get_filename_component(bin_dir "${Qt5Core_DIR}/../../../bin" ABSOLUTE)
  find_program(SEQUOIA_WINDEPLOYQT windeployqt.exe "${bin_dir}")
  if(NOT(SEQUOIA_WINDEPLOYQT))
    message(FATAL_ERROR "Cannot find the Qt5 deployment tool: 'windeployqt.exe' in ${bin_dir}")
  endif()
endif()

sequoia_export_package(
  NAME Qt5
  FOUND ON 
  LIBRARIES ${libraries}
  VERSION ${Qt5_VERSION}
  DEFINITIONS ${definitions}
  INCLUDE_DIRS ${include_dirs}
)