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

this_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# @brief Issue an error message to `stderr` and exit with 1
#
# @param $1   Message to print
fatal_error() {
  ERROR $1 
  exit 1
}

# @brief Install the <package>
#
# The following is assumed:
#   - The variable `<PACKGE>_VERSION` needs to exist.
#   - The script `install_<package>.sh` needs to exist and contain a function `install_<package>`.
#   - The function `install_<package>` has to take the <install-dir> as the first argument and 
#     `<PACKAGE>_VERSION` as the second.
#
# @param $1   Install directory
# @param $2   Package to install
# @param $*   Additional arguments passed to the install function
install_package() {
  local install_dir=$1
  shift
  local package=$1
  shift
  local args=$*
  local package_upper=$(echo $package | awk '{print toupper($0)}')
  local package_version_var="${package_upper}_VERSION"

  if [ -z ${!package_version_var+x} ]; then
    fatal_error "variable '$package_version_var' is not defined"
  fi

  source "$this_script_dir/install_${package}.sh" || exit 1
  "install_${package}" $install_dir ${!package_version_var} $args
}

# @brief Print the usage and exit with 0
print_help() {
cat << EOF
Usage: $0 --install-dir <install-dir> --build <package> [options]

  Options:
   -i, --install-dir <install-dir>     
        Each <package> is installed in 

          <install-dir>/<package>-<version> 
  
        where <version> must be an environment variable of the form 
        '<package>_VERSION' whith <package> being all uppercase. 
        (e.g if <package> is 'boost', we expect 'BOOST_VERSION' to be defined). 

    -b, --build <package>,[<package>...]
        Build and install the packages (in the given order).
    -h, --help        
        Print this help statement and exit.
EOF
  exit 0
}

# Setup logging
source $this_script_dir/b_log.sh
LOG_LEVEL_ALL

# Parse command-line
args=$(getopt -o b:i:h:: -l build:,install-dir:,help:: -n 'installer' -- "$@")
eval set -- "$args"

if [ $? -ne 0 ]; then
  exit 1
fi

while true; do 
  case "$1" in
    -h|--h*) print_help; exit 0;;
    -b|--build) packages=$2; shift 2;;
    -i|--install-dir) install_dir=$2; shift 2;;
    --) shift; break ;;
    *) echo "$0: internal error." ; exit 1 ;;
  esac
done

if [ -z ${install_dir+x} ]; then
  >&2 echo "$0: error: missing command-line argument '--install-dir'"
  exit 1
fi

if [ -z ${packages+x} ]; then
  >&2 echo "$0: error: missing command-line argument '--build'"
  exit 1
fi

# Make sure install directory exists
mkdir -p ${install_dir}

# Export toolchain
export CXX=g++-5
export CC=gcc-5

# Build package(s)
IFS=', ' read -r -a split_package <<< "$packages"
for package in "${split_package[@]}"
do
  case $package in
    boost) install_package ${install_dir} boost atomic chrono date_time filesystem program_options \
                                          thread;;
    cmake) install_package ${install_dir} cmake;;
    glbinding) install_package ${install_dir} glbinding;;
    opencv) install_package ${install_dir} opencv;;
    *) 
      >&2 echo "$0: error: unknown package '$package'";
      exit 1;;
  esac
done
