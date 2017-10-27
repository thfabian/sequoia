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

include(SequoiaGenerateOptions)

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

sequoia_add_library(
  NAME SequoiaCore
  SOURCES AlignedADT.h
          ArrayRef.h
          Any.h
          Assert.cpp
          Assert.h
          Byte.h
          Casting.h
          Color.cpp
          Color.h
          Compiler.h
          ConcurrentADT.h
          DoubleBuffered.h          
          ErrorHandler.cpp
          ErrorHandler.h
          Exception.cpp
          Exception.h
          Export.h
          File.cpp
          File.h
          FileType.inc
          Format.h
          Future.h
          Hash.h
          Image.cpp         
          Image.h
          Listenable.h
          Logging.cpp
          Logging.h
          Memory.h
          MicroBenchmark.cpp
          MicroBenchmark.h
          Mutex.h
          NonCopyable.h
          Optional.h
          Options.cpp
          Options.h.in
          Options.inc.in
          Options2.cpp
          Options2.h
          OptionsType.inc
          Platform.cpp   
          Platform.h
          PrettyStackTrace.cpp       
          PrettyStackTrace.h
          Singleton.h
          SingletonManager.cpp
          SingletonManager.h
          STLExtras.h
          StringRef.cpp 
          StringRef.h
          StringSwitch.h
          StringUtil.cpp
          StringUtil.h
          Task.h
          Terminal.cpp  
          Terminal.h
          Unreachable.cpp         
          Unreachable.h
          Utf.h
          UtfString.cpp
          UtfString.h
          Version.cpp
          Version.h
          
          ${option_header}
          ${option_include}
          ${options}
  OBJECT
)

