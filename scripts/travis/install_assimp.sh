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

# @brief Install assimp
#
# @param $1   Install directory
# @param $2   assimp version triple (X.Y.Z)
function install_assimp() {
  pushd "$(pwd)"
  local start_time=$(date +%s)

  if [[ $# -lt 2 ]]; then
    fatal_error "argument mistmatch: ${FUNCNAME[0]} <install_prefix> <version>"
  fi

  local install_dir="$1"
  shift
  local assimp_version=$1
  shift
  
  local assimp_install_dir="$install_dir/assimp-$assimp_version"

  abort_and_cleanup() {
    rm -rf "$assimp_install_dir" && mkdir -p "$assimp_install_dir"
    fatal_error "$1"
  }

  NOTICE "${FUNCNAME[0]}: Installing assimp $assimp_version into \"$assimp_install_dir\""
  mkdir -p "${assimp_install_dir}"
  if [[ ! -z "$(ls -A ${assimp_install_dir})" ]]; then
    NOTICE "${FUNCNAME[0]}: Package already installed. Skipping."
  else
    local assimp_url=$(printf "https://github.com/assimp/assimp/archive/v%s.tar.gz"                \
                       ${assimp_version})

    NOTICE "${FUNCNAME[0]}: Downloading assimp $assimp_url ..."
    { wget --no-check-certificate -O - "${assimp_url}" |                                           \
      tar --strip-components=1 -xz -C "${assimp_install_dir}"; } ||                                \
      abort_and_cleanup "failed to download assimp from: $assimp_url"
    NOTICE "${FUNCNAME[0]}: Successfully downloaded $assimp_url"

    cd "${assimp_install_dir}"
    NOTICE "${FUNCNAME[0]}: Starting to build assimp ..."
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release                                                            \
             -DASSIMP_BUILD_TESTS:BOOL=OFF                                                         \
             -DASSIMP_BUILD_SAMPLES:BOOL=OFF                                                       \
             -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF                                                  \
             -DASSIMP_INSTALL_PDB:BOOL=OFF                                                         \
             -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT:BOOL=OFF                                      \
             -DASSIMP_BUILD_OBJ_IMPORTER:BOOL=ON                                                   \
             -DCMAKE_INSTALL_PREFIX="${assimp_install_dir}"                                        \
          || abort_and_cleanup "Failed to configure assimp"
    make -j2 install || abort_and_cleanup "Failed to build assimp"
  fi
  
  local elapsed_time=$(expr $(date +%s) - $start_time)
  NOTICE $(printf "${FUNCNAME[0]}: Successfully installed assimp %s (took %dm %ds)\n"              \
           $assimp_version $(($elapsed_time%3600/60)) $(($elapsed_time%60)))

  export assimp_DIR="${assimp_install_dir}"
  popd
}
