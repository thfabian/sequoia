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

include(SequoiaIncludeGuard)
sequoia_include_guard()

include(SequoiaMakeStringPair)

#.rst:
# .. _sequoia_create_package_string:
#
# sequoia_create_package_string
# -----------------------------
#
# Create a package string including the location of the library/include directory and version 
# string. Note that the package has to be exported via :ref:`sequoia_export_package`.
#
# .. code-block:: cmake
#
#   sequoia_create_package_string(NAME PACKAGE_STRING [NOT_FOUND_STRING])
#
# ``NAME``
#   Name of the package.
# ``PACKAGE_STRING``
#   Returned package string.
# ``NOT_FOUND_STRING``
#   String to print of the package was not found, defaults to "Not FOUND" (optional).
#
# Example
# ^^^^^^^
# .. code-block:: cmake
#
#   sequoia_export_package(Foo FOUND TRUE LIBRARIES "/usr/lib/libfoo.so" VERSION "1.2.3")
#   sequoia_create_package_string(Foo FooPackageStr)
#   message(${FooPackageStr})
#
function(sequoia_create_package_string NAME PACKAGE_STRING)
  set(not_found_string "NOT found")
  if(NOT("${ARGV2}" STREQUAL ""))
    set(not_found_string "${ARGV2}")
  endif()

  string(TOUPPER ${NAME} package)
  if(NOT(DEFINED SEQUOIA_${package}_FOUND) OR NOT(${SEQUOIA_${package}_FOUND}))
     set(info "${not_found_string}")
  else()
    if(DEFINED SEQUOIA_${package}_INCLUDE_DIRS)
      list(GET  SEQUOIA_${package}_INCLUDE_DIRS 0 inc)
      set(info "${inc}")
    elseif(DEFINED SEQUOIA_${package}_EXECUTABLE)
      list(GET SEQUOIA_${package}_EXECUTABLE 0 exe)
      set(info "${exe}")
    else()
      set(info "found")
    endif()

    if(DEFINED SEQUOIA_${package}_VERSION)
      set(info "${info} (ver ${SEQUOIA_${package}_VERSION})")
    endif()
  endif()

  sequoia_make_string_pair(${NAME} ${info} 20 out_string)
  set(${PACKAGE_STRING} ${out_string} PARENT_SCOPE)
endfunction()
