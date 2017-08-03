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

# @brief Install the Boost libraries
#
# This function relies on $CXX pointing to a GNU compiler.
#
# @param $1   Install directory
# @param $2   Boost version triple (X.Y.Z)
# @param $*   Boost components to build (e.g program_options)
install_boost() {
  pushd $(pwd)
  local start_time=$(date +%s)

  if [[ $# -lt 3 ]]; then
    fatal_error "argument mistmatch: ${FUNCNAME[0]} <install_prefix> <version> <components...>"
  fi

  local install_dir=$1
  shift
  local boost_version=$1
  shift
  
  local boost_install_dir=$install_dir/boost-$boost_version
  local boost_version_underscore=${boost_version//\./_}
  
  abort_and_cleanup() {
    rm -rf $boost_install_dir && mkdir -p $boost_install_dir
    fatal_error "$1"
  }

  NOTICE "${FUNCNAME[0]}: Installing boost $boost_version into \"$boost_install_dir\" ..."
  mkdir -p ${boost_install_dir}
  if [[ ! -z "$(ls -A ${boost_install_dir})" ]]; then
    NOTICE "${FUNCNAME[0]}: Package already installed. Skipping."
  else
    local boost_url=$(printf "http://sourceforge.net/projects/boost/files/boost/%s/boost_%s.tar.gz"\
                      ${boost_version} ${boost_version_underscore})

    NOTICE "${FUNCNAME[0]}: Downloading boost $boost_url ..."
    { wget --no-check-certificate -O - ${boost_url} |                                              \
      tar --strip-components=1 -xz -C ${boost_install_dir}; } ||                                   \
      abort_and_cleanup "Failed to download boost from: $boost_url"
    NOTICE "${FUNCNAME[0]}: Successfully downloaded $boost_url"

    cd ${boost_install_dir}
    local gcc_version=$($CXX -dumpversion)
    NOTICE "${FUNCNAME[0]}: Building boost with toolset gcc : $gcc_version ..."
    echo "using gcc : ${gcc_version} : ${CXX} ;" > user-config.jam
    
    NOTICE "${FUNCNAME[0]}: Building components: $*"
    local boost_components_arg=$*
    for component in $*; do
      boost_components_arg="$boost_components_arg --with-$component"
    done

    NOTICE "${FUNCNAME[0]}: Starting to build boost ..."
    ./bootstrap.sh || abort_and_cleanup "Failed to configure boost"
    ./b2 -j2 --toolset=gcc-${gcc_version} --prefix=${boost_install_dir}                            \
             --user-config=${boost_install_dir}/user-config.jam                                    \
             ${boost_components_arg} install || abort_and_cleanup "Failed to build boost"
  fi
  
  local elapsed_time=$(expr $(date +%s) - $start_time)
  NOTICE $(printf "${FUNCNAME[0]}: Successfully installed boost $boost_version (took %dm %ds)\n"   \
           $(($elapsed_time%3600/60)) $(($elapsed_time%60)))

  export BOOST_ROOT="${boost_install_dir}"
  popd
}
