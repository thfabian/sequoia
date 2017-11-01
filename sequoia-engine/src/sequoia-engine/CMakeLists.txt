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

# Link objects into a single library
sequoia_combine_libraries(
  NAME SequoiaEngine
  OBJECTS SequoiaEngineCoreObjects
          SequoiaEngineMathObjects
          SequoiaEngineRenderObjects
          SequoiaEngineDriverObjects
          SequoiaEngineGameObjects
  VERSION ${SEQUOIA_ENGINE_VERSION}
  DEPENDS ${SEQUOIA_ENGINE_EXTERNAL_LIBRARIES}
  INSTALL_DESTINATION ${SEQUOIA_ENGINE_INSTALL_LIB_DIR}
)

install(
  EXPORT SequoiaEngineTargets 
  NAMESPACE sequoia:: 
  DESTINATION ${SEQUOIA_ENGINE_INSTALL_CMAKE_DIR}
  EXPORT_LINK_INTERFACE_LIBRARIES
)

if(SEQUOIA_ENGINE_TESTING)
  add_subdirectory(Unittest)
endif()

# TODO: remove 
sequoia_add_executable(
  NAME SequoiaStandalone
  WIN32_APPLICATION
  SOURCES Driver/Main.cpp
  DEPENDS SequoiaEngineStatic
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin
)
