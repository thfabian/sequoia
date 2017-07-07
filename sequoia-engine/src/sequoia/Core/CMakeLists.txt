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

# Generate Config.h
sequoia_configure_file(Config.h.cmake)

# Generate the Options.h and Options.inc
set(option_header ${CMAKE_CURRENT_BINARY_DIR}/Options.h)
set(option_include ${CMAKE_CURRENT_BINARY_DIR}/Options.inc)
set(options
  ${CMAKE_SOURCE_DIR}/src/sequoia/Core/Options.json
  ${CMAKE_SOURCE_DIR}/src/sequoia/Render/Options.json
  ${CMAKE_SOURCE_DIR}/src/sequoia/Driver/Options.json
  ${CMAKE_SOURCE_DIR}/src/sequoia/Math/Options.json
)

sequoia_generate_options(
  INPUT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/Options.h.in
  INPUT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Options.inc.in
  OUTPUT_HEADER ${option_header}
  OUTPUT_INCLUDE ${option_include}
  OPTIONS ${options}
)

sequoia_add_objects(
  NAME SequoiaCore
  SOURCES AlignedADT.h
          ArrayRef.h
          Any.h
          Assert.h
          Assert.cpp
          Byte.h
          Casting.h
          Compiler.h
          Color.h
          Color.cpp
          DoubleBuffered.h          
          Export.h
          ErrorHandler.h
          ErrorHandler.cpp
          Exception.h
          Exception.cpp
          File.h
          File.cpp
          FileType.inc
          Format.h
          HashCombine.h
          Image.h
          Image.cpp         
          Listenable.h
          Logging.h
          Logging.cpp
          NonCopyable.h
          MicroBenchmark.h
          MicroBenchmark.cpp
          Memory.h
          Mutex.h
          Options.h.in
          Options.inc.in
          Options.cpp
          Platform.h
          Platform.cpp   
          PrettyStackTrace.h
          PrettyStackTrace.cpp       
          Singleton.h
          SingletonManager.h
          SingletonManager.cpp
          StringSwitch.h
          StringRef.h
          StringRef.cpp 
          StringUtil.h
          StringUtil.cpp
          STLExtras.h
          Terminal.h
          Terminal.cpp        
          Unreachable.h
          Unreachable.cpp         
          Utf.h
          UtfString.h
          UtfString.cpp
          Version.h
          Version.cpp
          
          ${option_header}
          ${option_include}
          ${options}
)

