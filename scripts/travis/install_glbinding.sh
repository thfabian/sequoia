#!/bin/bash 
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

# @brief Install glbinding
#
# @param $1   Install directory
# @param $2   glbinding version triple (X.Y.Z)
install_glbinding() {
  pushd $(pwd)
  local start_time=$(date +%s)

  if [[ $# -lt 2 ]]; then
    fatal_error "argument mistmatch: ${FUNCNAME[0]} <install_prefix> <version>"
  fi

  local install_dir=$1
  shift
  local glbinding_version=$1
  shift
  
  local glbinding_install_dir=$install_dir/glbinding-$glbinding_version

  abort_and_cleanup() {
    rm -rf $glbinding_install_dir && mkdir -p $glbinding_install_dir 
    fatal_error "$1"
  }

  NOTICE "${FUNCNAME[0]}: Installing glbinding $glbinding_version into \"$glbinding_install_dir\""
  mkdir -p ${glbinding_install_dir}
  if [[ ! -z "$(ls -A ${glbinding_install_dir})" ]]; then
    NOTICE "${FUNCNAME[0]}: Package already installed. Skipping."
  else
    local glbinding_url=$(printf "https://github.com/cginternals/glbinding/archive/v%s.tar.gz"     \
                      ${glbinding_version})

    NOTICE "${FUNCNAME[0]}: Downloading glbinding $glbinding_url ..."
    { wget --no-check-certificate -O - ${glbinding_url} |                                          \
      tar --strip-components=1 -xz -C ${glbinding_install_dir}; } ||                               \
      abort_and_cleanup "failed to download glbinding from: $glbinding_url"
    NOTICE "${FUNCNAME[0]}: Successfully downloaded $glbinding_url"

    cd ${glbinding_install_dir}
    NOTICE "${FUNCNAME[0]}: Starting to build glbinding ..."
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DOPTION_BUILD_DOCS:BOOL=OFF                               \
             -DOPTION_BUILD_EXAMPLES:BOOL=OFF -DOPTION_BUILD_TESTS:BOOL=OFF                        \
             -DCMAKE_INSTALL_PREFIX=${glbinding_install_dir}                                       \
          || abort_and_cleanup "Failed to configure glbinding"
    make -j2 install || abort_and_cleanup "Failed to build glbinding"
  fi
  
  local elapsed_time=$(expr $(date +%s) - $start_time)
  NOTICE $(printf "${FUNCNAME[0]}: Successfully installed glbinding %s (took %dm %ds)\n"           \
           $glbinding_version $(($elapsed_time%3600/60)) $(($elapsed_time%60)))

  export GLBINDING_ROOT="${glbinding_install_dir}"
  popd
}
