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

set(png_extra_args)
set(png_depends)

if(TARGET zlib)
  list(APPEND png_depends zlib)
endif()

if(DEFINED ZLIB_ROOT)
  list(APPEND png_extra_args -DZLIB_ROOT=${ZLIB_ROOT})
endif()

ExternalProject_Add(
  libpng
  DOWNLOAD_DIR ${download_dir}
  URL ${png_url}
  URL_MD5 ${png_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/png"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/png"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DPNG_TESTS:BOOL=OFF
    ${png_extra_args}
  DEPENDS
    ${png_depends}
)

ExternalProject_Get_Property(libpng install_dir)
set(LIBPNG_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DLIBPNG_ROOT:PATH=${LIBPNG_ROOT}")

