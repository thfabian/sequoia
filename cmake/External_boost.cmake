##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##
##
## Build boost via its bootstrap script. The build tree cannot contain a space. This boost b2 build 
## system yields errors with spaces in the name of the build dir.
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
    set(_toolset "msvc-14.0")
  else()
    message(FATAL_ERROR "Only Microsoft Visual C 14.0 is supported (Visual Studio 2015)")
  endif()
  
  list(APPEND boost_with_args "--build-type=complete" "--layout=tagged" "toolset=${_toolset}")

  set(boost_cmds
    CONFIGURE_COMMAND bootstrap.bat
    BUILD_COMMAND b2 address-model=${am} ${boost_with_args}
    INSTALL_COMMAND b2 address-model=${am} ${boost_with_args} --prefix=<INSTALL_DIR> install
  )
else()
  set(boost_cmds
    CONFIGURE_COMMAND ./bootstrap.sh --prefix=<INSTALL_DIR>
    BUILD_COMMAND ./b2 address-model=${am} ${boost_with_args}
    INSTALL_COMMAND ./b2 address-model=${am} ${boost_with_args} install
  )
endif()

ExternalProject_Add(boost
  DOWNLOAD_DIR ${download_dir}
  URL ${boost_url}
  URL_MD5 ${boost_md5}
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/boost"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  ${boost_cmds}
  BUILD_IN_SOURCE 1
)

ExternalProject_Get_Property(boost install_dir)
set(BOOST_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS
  "-DBOOST_ROOT:PATH=${BOOST_ROOT}"
  "-DBoost_INCLUDE_DIR:PATH=${BOOST_ROOT}/include"
  "-DBOOST_LIBRARYDIR:PATH=${BOOST_ROOT}/lib"
  "-DBoost_NO_SYSTEM_PATHS:BOOL=ON")

