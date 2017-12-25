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

sequoia_configure_file(Config.h.cmake)

sequoia_add_library(
  NAME SequoiaEngineCore
  SOURCES AlignedADT.h
          Any.h
          ArrayRef.h
          Assert.cpp
          Assert.h
          Byte.h
          Casting.h
          Color.cpp
          Color.h
          CommandLine.cpp
          CommandLine.h
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
          Options.h
          Platform.cpp   
          Platform.h
          PreprocessorUtil.h
          PrettyStackTrace.cpp       
          PrettyStackTrace.h
          Singleton.h
          STLExtras.h
          StringRef.cpp 
          StringRef.h
          StringSwitch.h
          StringUtil.cpp
          StringUtil.h
          Task.h
          Terminal.cpp  
          Terminal.h
          Timer.h
          Unreachable.cpp         
          Unreachable.h
          Utf.h
          UtfString.cpp
          UtfString.h
          Version.cpp
          Version.h
          Win32Console.cpp
          Win32Console.h
          Xml.h
  OBJECT
)

