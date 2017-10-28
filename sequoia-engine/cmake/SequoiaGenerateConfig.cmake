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

  # Export configuration
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

  # Install cmake scripts
  install(FILES ${version_file} ${config_file} DESTINATION ${SEQUOIA_ENGINE_INSTALL_CMAKE_DIR})
endfunction()