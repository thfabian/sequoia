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

add_subdirectory(Core)
add_subdirectory(Driver)

# Link SequoiaEditor
sequoia_add_executable(
  NAME SequoiaEditor
  SOURCES Driver/Main.cpp
  DEPENDS SequoiaEditorCoreObjects
          ${SEQUOIA_EDITOR_LIBRARIES}
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin
  INSTALL_DESTINATION ${SEQUOIA_EDITOR_INSTALL_BIN_DIR}
  WIN32_APPLICATION
)

# Copy Qt5 DLLs
if(WIN32)
  add_custom_command(
    TARGET SequoiaEditor POST_BUILD
    COMMAND "${SEQUOIA_WINDEPLOYQT}" $<TARGET_FILE:SequoiaEditor> --no-compiler-runtime
    COMMENT "Deploying Qt5"
  )
endif()