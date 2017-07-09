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

# Compile object libraries
add_subdirectory(Core)
add_subdirectory(Math)
add_subdirectory(Render)
add_subdirectory(Driver)
add_subdirectory(Game)

set(UnittestTarget)
if(SEQUOIA_TESTING)
  add_subdirectory(Unittest)
  set(UnittestTarget SequoiaUnittest)
endif()

# Link objects into a single library
sequoia_link_objects(
  NAME Sequoia
  SOURCES SequoiaCore
          SequoiaMath
          SequoiaRender
          SequoiaDriver
          SequoiaGame
          ${UnittestTarget}
  DEPENDS ${SEQUOIA_EXTERNAL_LIBRARIES}
)

# Compile standalone driver
sequoia_add_executable(
  NAME SequoiaStandalone
  WIN32_APPLICATION
  SOURCES Driver/Main.cpp
  DEPENDS Sequoia
)

