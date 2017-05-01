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

# Generate Config.h
sequoia_configure_file(Config.h.cmake)

# Generate the Options.h and Options.inc
set(option_header ${CMAKE_CURRENT_BINARY_DIR}/Options.h)
set(option_include ${CMAKE_CURRENT_BINARY_DIR}/Options.inc)
sequoia_generate_options(
  INPUT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/Options.h.in
  INPUT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Options.inc.in
  OUTPUT_HEADER ${option_header}
  OUTPUT_INCLUDE ${option_include}
  OPTIONS 
    ${CMAKE_SOURCE_DIR}/src/sequoia/Core/Options.json
)

sequoia_add_library(
  NAME SequoiaCore
  SOURCES ArrayRef.h
          Any.h
          Assert.h
          Assert.cpp 
          Export.h
          ErrorHandler.h
          ErrorHandler.cpp
          Format.h          
          Compiler.h
          NString.h
          NString.cpp
          NPath.h
          NonCopyable.h
          Platform.h
          Singleton.h
          SingletonManager.h
          SingletonManager.cpp
          StringSwitch.h
          StringRef.h
          StringRef.cpp          
          Utf.h
          UtfString.h
          UtfString.cpp
          Version.h
          Version.cpp

          ${option_header}
          ${option_include}
  DEPENDS ${SEQUOIA_EXTERNAL_LIBRARIES}
)

