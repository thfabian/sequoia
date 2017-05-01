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

#.rst:
# FindGLFW
# --------
#
# This script locates glfw3. This script makes use of the standard find_package arguments of
# ``VERSION``, ``REQUIRED`` and ``QUIET``. GLFW_FOUND will report if an acceptable version of
# glfw was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   GLFW_FOUND            - System has the glfw headers
#   GLFW_VERSION          - Version of the library
#   GLFW_INCLUDE_DIRS     - The location of glfw headers (i.e the header glfw3.h is in 
#                           ${GLFW_INCLUDE_DIRS}/GLFW/glfw3.h)
#   GLFW_LIBRARIES        - The list of libraries to link against (includes external libraries 
#                           such as X11, Cocoa etc.)
#
# Hints
# ^^^^^
#
# Set ``GLFW_ROOT`` to a directory that contains a glfw installation.

find_path(GLFW_INCLUDE_DIRS 
  NAMES
    GLFW/glfw3.h
  HINTS
    "${GLFW_ROOT}/include"
    "$ENV{GLFW_ROOT}/include"
  PATHS
    "$ENV{PROGRAMFILES}/GLFW/include"
    "${OPENGL_INCLUDE_DIR}"
    /usr/openwin/share/include
    /usr/openwin/include
    /usr/X11R6/include
    /usr/include/X11
    /opt/graphics/OpenGL/include
    /opt/graphics/OpenGL/contrib/libglfw
    /usr/local/include
    /usr/include/GL
    /usr/include
  DOC 
    "The directory where GLFW/glfw3.h resides"
)

if(WIN32)
  if(CYGWIN)
    find_library(GLFW_glfw_LIBRARY 
        NAMES
          glfw32
        HINTS
          "${GLFW_ROOT}/lib"
          "${GLFW_ROOT}/lib/x64"
          "$ENV{GLFW_ROOT}/lib"
        PATHS
          "${OPENGL_LIBRARY_DIR}"
          /usr/lib
          /usr/lib/w32api
          /usr/local/lib
          /usr/X11R6/lib
        DOC 
          "The GLFW library"
    )
  else()
    find_library(GLFW_glfw_LIBRARY
      NAMES 
        glfw32 
        glfw32s 
        glfw3
        glfw3dll
      HINTS
        "${GLFW_ROOT}/lib"
        "${GLFW_ROOT}/lib/x64"
        "${GLFW_ROOT}/lib-msvc110"
        "${GLFW_ROOT}/lib-vc2012"
        "$ENV{GLFW_ROOT}/lib"
        "$ENV{GLFW_ROOT}/lib/x64"
        "$ENV{GLFW_ROOT}/lib-msvc110"
        "$ENV{GLFW_ROOT}/lib-vc2012"
      PATHS
        "$ENV{PROGRAMFILES}/GLFW/lib"
        "${OPENGL_LIBRARY_DIR}"
      DOC 
        "The GLFW library"
    )
  endif()
else()
  if(APPLE)
    find_library(GLFW_glfw_LIBRARY
      NAMES 
        glfw
        glfw3
      HINTS
        "${GLFW_ROOT}/lib"
        "${GLFW_ROOT}/lib/cocoa"
        "$ENV{GLFW_ROOT}/lib"
        "$ENV{GLFW_ROOT}/lib/cocoa"
      PATHS
        /usr/local/lib
    )
    set(GLFW_cocoa_LIBRARY "-framework Cocoa" CACHE STRING "Cocoa framework for OSX")
    set(GLFW_corevideo_LIBRARY "-framework CoreVideo" CACHE STRING "CoreVideo framework for OSX")
    set(GLFW_iokit_LIBRARY "-framework IOKit" CACHE STRING "IOKit framework for OSX")
  else()
    # (*)NIX
    
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)

    if(NOT X11_Xrandr_FOUND)
      message(FATAL_ERROR "Xrandr library not found - required for GLFW")
    endif()

    if(NOT X11_xf86vmode_FOUND)
      message(FATAL_ERROR "xf86vmode library not found - required for GLFW")
    endif()

    if(NOT X11_Xcursor_FOUND)
      message(FATAL_ERROR "Xcursor library not found - required for GLFW")
    endif()

    if(NOT X11_Xinerama_FOUND)
      message(FATAL_ERROR "Xinerama library not found - required for GLFW")
    endif()

    if(NOT X11_Xi_FOUND)
      message(FATAL_ERROR "Xi library not found - required for GLFW")
    endif()

    list(APPEND GLFW_x11_LIBRARY 
     "${X11_Xrandr_LIB}" 
     "${X11_Xxf86vm_LIB}" 
     "${X11_Xcursor_LIB}" 
     "${X11_Xinerama_LIB}" 
     "${X11_Xi_LIB}" 
     "${X11_LIBRARIES}" 
     "${CMAKE_THREAD_LIBS_INIT}" 
     -lrt 
     -ldl
    )

    find_library(GLFW_glfw_LIBRARY
      NAMES 
        glfw
        glfw3
      HINTS
        "${GLFW_ROOT}/lib"
        "$ENV{GLFW_ROOT}/lib"
        "${GLFW_ROOT}/lib/x11"
        "$ENV{GLFW_ROOT}/lib/x11"
      PATHS
        /usr/lib64
        /usr/lib
        /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
        /usr/local/lib64
        /usr/local/lib
        /usr/local/lib/${CMAKE_LIBRARY_ARCHITECTURE}
        /usr/openwin/lib
        /usr/X11R6/lib
      DOC 
        "The GLFW library"
    )
  endif()
endif()

set(GLFW_FOUND OFF)

if(GLFW_INCLUDE_DIRS)
    if(GLFW_glfw_LIBRARY)
      set(GLFW_LIBRARIES "${GLFW_glfw_LIBRARY}"
                         "${GLFW_x11_LIBRARY}"
                         "${GLFW_cocoa_LIBRARY}"
                         "${GLFW_iokit_LIBRARY}"
                         "${GLFW_corevideo_LIBRARY}" )
      set(GLFW_FOUND ON)
      set(GLFW_LIBRARY "${GLFW_LIBRARIES}")
      set(GLFW_INCLUDE_PATH "${GLFW_INCLUDE_DIRS}")
    endif(GLFW_glfw_LIBRARY)

    function(parseVersion FILENAME VARNAME)
      set(PATTERN "^#define ${VARNAME}.*$")
      file(STRINGS "${GLFW_INCLUDE_DIRS}/${FILENAME}" TMP REGEX ${PATTERN})
      string(REGEX MATCHALL "[0-9]+" TMP ${TMP})
      set(${VARNAME} ${TMP} PARENT_SCOPE)
    endfunction()

    if(EXISTS "${GLFW_INCLUDE_DIRS}/GLFW/glfw3.h")
      parseVersion(GLFW/glfw3.h GLFW_VERSION_MAJOR)
      parseVersion(GLFW/glfw3.h GLFW_VERSION_MINOR)
      parseVersion(GLFW/glfw3.h GLFW_VERSION_REVISION)
    endif()

    if(${GLFW_VERSION_MAJOR} OR ${GLFW_VERSION_MINOR} OR ${GLFW_VERSION_REVISION})
      set(GLFW_VERSION "${GLFW_VERSION_MAJOR}.${GLFW_VERSION_MINOR}.${GLFW_VERSION_REVISION}")
      set(GLFW_VERSION_STRING "${GLFW_VERSION}")
      mark_as_advanced(GLFW_VERSION)
    endif()
endif(GLFW_INCLUDE_DIRS)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLFW 
  REQUIRED_VARS
    GLFW_INCLUDE_DIRS
    GLFW_LIBRARIES
  FAIL_MESSAGE "Could NOT find GLFW. (Try setting GLFW_ROOT in the env)"
  VERSION_VAR
    GLFW_VERSION
)

mark_as_advanced(
  GLFW_INCLUDE_DIRS
  GLFW_LIBRARIES
  GLFW_glfw_LIBRARY
  GLFW_cocoa_LIBRARY
  GLFW_corevideo_LIBRARY
  GLFW_iokit_LIBRARY
  GLFW_x11_LIBRARY
)

if(NOT(GLFW_FOUND) AND GLFW_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find GLFW. (Try setting GLFW_ROOT in the env)")
endif()

