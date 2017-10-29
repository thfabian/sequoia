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

set(cmake_args
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    ${SEQUOIA_EXTERNAL_PROJECTS_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -Dprotobuf_BUILD_EXAMPLES=OFF
    -Dprotobuf_BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -Dprotobuf_BUILD_TESTS=OFF
    -Dprotobuf_INSTALL_EXAMPLES=OFF
)

ExternalProject_Add(protobuf
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}"
  URL ${protobuf_url}
  URL_MD5 ${protobuf_md5}
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/protobuf"
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} <SOURCE_DIR>/cmake ${cmake_args}
)

ExternalProject_Get_Property(protobuf install_dir)
if(WIN32)
  set(Protobuf_DIR "${install_dir}/cmake" CACHE INTERNAL "")
else()
  set(Protobuf_DIR "${install_dir}/lib/cmake/protobuf" CACHE INTERNAL "")
endif()

sequoia_append_and_export_variable(
   SEQUOIA_EXTERNAL_PROJECTS_CMAKE_ARGS 
  "-DProtobuf_DIR:PATH=${Protobuf_DIR}" 
)
