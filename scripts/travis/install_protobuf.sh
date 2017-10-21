#!/bin/bash 
##===-------------------------------------------------------------------------------*- Bash -*-===##
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

# @brief Install protobuf
#
# @param $1   Install directory
# @param $2   protobuf version triple (X.Y.Z)
function install_protobuf() {
  pushd "$(pwd)"
  local start_time=$(date +%s)

  if [[ $# -lt 2 ]]; then
    fatal_error "argument mistmatch: ${FUNCNAME[0]} <install_prefix> <version>"
  fi

  local install_dir="$1"
  shift
  local protobuf_version=$1
  shift
  
  local protobuf_install_dir="$install_dir/protobuf-$protobuf_versio"n

  abort_and_cleanup() {
    rm -rf "$protobuf_install_dir" && mkdir -p "$protobuf_install_dir" 
    fatal_error "$1"
  }

  NOTICE "${FUNCNAME[0]}: Installing protobuf $protobuf_version into \"$protobuf_install_dir\""
  mkdir -p ${protobuf_install_dir}
  if [[ ! -z "$(ls -A ${protobuf_install_dir})" ]]; then
    NOTICE "${FUNCNAME[0]}: Package already installed. Skipping."
  else
    local protobuf_url=$(printf "https://github.com/google/protobuf/archive/v%s.tar.gz"            \
                      ${protobuf_version})

    NOTICE "${FUNCNAME[0]}: Downloading protobuf $protobuf_url ..."
    { wget --no-check-certificate -O - "${protobuf_url}" |                                         \
      tar --strip-components=1 -xz -C "${protobuf_install_dir}"; } ||                              \
      abort_and_cleanup "failed to download protobuf from: $protobuf_url"
    NOTICE "${FUNCNAME[0]}: Successfully downloaded $protobuf_url"

    cd "${protobuf_install_dir}"
    NOTICE "${FUNCNAME[0]}: Starting to build protobuf ..."
    mkdir build && cd build
    cmake ../cmake/ -DCMAKE_BUILD_TYPE=Release                                                     \
                    -Dprotobuf_BUILD_TESTS=OFF                                                     \
                    -Dprotobuf_BUILD_EXAMPLES=OFF                                                  \
                    -DCMAKE_INSTALL_PREFIX="${protobuf_install_dir}"                               \
          || abort_and_cleanup "Failed to configure protobuf"
    make -j2 install || abort_and_cleanup "Failed to build protobuf"
  fi
  
  local elapsed_time=$(expr $(date +%s) - $start_time)
  NOTICE $(printf "${FUNCNAME[0]}: Successfully installed protobuf %s (took %dm %ds)\n"            \
           $protobuf_version $(($elapsed_time%3600/60)) $(($elapsed_time%60)))

  export Protobuf_DIR="${protobuf_install_dir}/lib/cmake/protobuf"
  popd
}