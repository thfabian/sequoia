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

include(CMakePackageConfigHelpers)

# sequoia_engine_generate_config
# ------------------------------
#
# Create the CMake packge configuration for installation.
#
function(sequoia_engine_generate_config)
  # Export version
  set(version_file 
      "${CMAKE_BINARY_DIR}/${SEQUOIA_ENGINE_INSTALL_CMAKE_DIR}/SequoiaEngineConfigVersion.cmake")
  write_basic_package_version_file(
    ${version_file}
    VERSION ${SEQUOIA_ENGINE_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

  set(SEQUOIA_ENGINE_INSTALL_ROOT "")

  # Export relevant options options
  set(CONFIG_SEQUOIA_ENGINE_OPTIONS 
      "${CONFIG_SEQUOIA_ENGINE_OPTIONS}set(SEQUOIA_ENGINE_ASSERTS ${SEQUOIA_ENGINE_ASSERTS})\n")
  set(CONFIG_SEQUOIA_ENGINE_OPTIONS 
      "${CONFIG_SEQUOIA_ENGINE_OPTIONS}set(SEQUOIA_ENGINE_OPTIMIZE ${SEQUOIA_ENGINE_OPTIMIZE})")

  # Export include directories
  set(include_dirs "${SEQUOIA_ENGINE_EXTERNAL_INCLUDE_DIRS}")
  list(REMOVE_DUPLICATES include_dirs)
  set(CONFIG_SEQUOIA_ENGINE_INCLUDE_DIRS "set(SEQUOIA_ENGINE_INCLUDE_DIRS ${include_dirs})")

  # Export definitions
  set(definitions "${SEQUOIA_ENGINE_EXTERNAL_DEFINITIONS}")
  list(REMOVE_DUPLICATES definitions)
  set(CONFIG_SEQUOIA_ENGINE_DEFINITIONS "set(SEQUOIA_ENGINE_DEFINITIONS ${definitions})")

  # Export compile definitions
  set(compile_definitions "${CMAKE_CXX_FLAGS}")
  list(REMOVE_DUPLICATES compile_definitions)
  set(CONFIG_SEQUOIA_ENGINE_COMPILE_DEFINITIONS 
      "set(SEQUOIA_ENGINE_COMPILE_DEFINITIONS ${compile_definitions})")

  # Generate configuration
  set(config_file 
     "${CMAKE_BINARY_DIR}/${SEQUOIA_ENGINE_INSTALL_CMAKE_DIR}/SequoiaEngineConfig.cmake")
  configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/templates/SequoiaEngineConfig.cmake.in" 
    ${config_file}
    INSTALL_DESTINATION ${SEQUOIA_ENGINE_INSTALL_CMAKE_DIR}
    PATH_VARS 
      SEQUOIA_ENGINE_INSTALL_BIN_DIR
      SEQUOIA_ENGINE_INSTALL_CMAKE_DIR
      SEQUOIA_ENGINE_INSTALL_INCLUDE_DIR
      SEQUOIA_ENGINE_INSTALL_LIB_DIR
      SEQUOIA_ENGINE_INSTALL_ROOT
  )

  # Install the generated CMake files
  install(FILES ${version_file} ${config_file} DESTINATION ${SEQUOIA_ENGINE_INSTALL_CMAKE_DIR})
endfunction()
