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

find_package(Protobuf 3.4 NO_MODULE REQUIRED)

get_property(Protobuf_INCLUDE_DIRS 
  TARGET protobuf::libprotobuf 
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES
)

get_property(Protobuf_PROTOC_EXECUTABLE 
  TARGET protobuf::protoc 
  PROPERTY LOCATION
)

sequoia_export_package(
  NAME Protobuf
  FOUND ${Protobuf_FOUND}
  EXECUTABLE "${Protobuf_PROTOC_EXECUTABLE}"
  INCLUDE_DIRS "${Protobuf_INCLUDE_DIRS}"
  LIBRARIES "protobuf::libprotobuf"
  VERSION "${Protobuf_VERSION}"
)
