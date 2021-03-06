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

if("${CMAKE_CURRENT_BINARY_DIR}" MATCHES " ")
  message(FATAL_ERROR "cannot use boost bootstrap with a space in the name of the build dir")
endif()

if(NOT DEFINED use_bat)
  if(WIN32)
    set(use_bat 1)
  else()
    set(use_bat 0)
  endif()
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(am 64)
else()
  set(am 32)
endif()

foreach(component ${boost_components})
  list(APPEND boost_with_args --with-${component})
endforeach()

if(BUILD_SHARED_LIBS)
  list(APPEND boost_with_args "link=shared")
else()
  list(APPEND boost_with_args "link=static")
endif()

if(use_bat)
  if(MSVC14)
    set(toolset "msvc-14.0")
  else()
    message(FATAL_ERROR "Only MSVC14 is supported")
  endif()

  # Always build all runtimes (MD and MT)
  list(APPEND boost_with_args "--build-type=complete" "--layout=tagged" "toolset=${toolset}")

  set(boost_cmds
    CONFIGURE_COMMAND "<SOURCE_DIR>/bootstrap.bat"
    BUILD_COMMAND "<SOURCE_DIR>/b2.exe" -j4 address-model=${am} ${boost_with_args}
    INSTALL_COMMAND "<SOURCE_DIR>/b2.exe" -j4 address-model=${am} ${boost_with_args} --prefix=<INSTALL_DIR> install
  )
else()
  set(boost_cmds
    CONFIGURE_COMMAND ./bootstrap.sh --prefix=<INSTALL_DIR>
    BUILD_COMMAND ./b2 address-model=${am} ${boost_with_args}
    INSTALL_COMMAND ./b2 address-model=${am} ${boost_with_args} install
  )
endif()

ExternalProject_Add(boost
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/boost"
  URL ${boost_url}
  URL_MD5 ${boost_md5}
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/boost"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/boost"
  ${boost_cmds}
  BUILD_IN_SOURCE 1
)

ExternalProject_Get_Property(boost install_dir)
set(BOOST_ROOT "${install_dir}" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE boost 
  CMAKE_ARGS 
    "-DBOOST_ROOT:PATH=${BOOST_ROOT}"
    "-DBoost_INCLUDE_DIR:PATH=${BOOST_ROOT}/include"
    "-DBOOST_LIBRARYDIR:PATH=${BOOST_ROOT}/lib"
    "-DBoost_NO_SYSTEM_PATHS:BOOL=ON"
)