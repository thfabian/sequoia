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

this_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Setup dependencies
source "$this_script_dir/install.sh"
install_driver -i ${CACHE_DIR} -b cmake,boost,glbinding,opencv,assimp                              \
               -c boost:atomic,chrono,date_time,filesystem,program_options,thread

if [ ! -z ${CLANG_VERSION+x} ]; then
  install_driver -i "${CACHE_DIR}" -b clang
fi

# Make sure C++ and C compilers are set correctly
export CXX=${CXX_COMPILER}
export CC=${C_COMPILER}

$CC --version
$CXX --version
python3.5 --version

# Build sequoia
pushd "$(pwd)"
mkdir build && cd build
cmake .. -DBOOST_ROOT:PATH="$BOOST_ROOT"                                                           \
         -DGLBINDING_ROOT:PATH="$GLBINDING_ROOT"                                                   \
         -DOpenCV_DIR:PATH="$OpenCV_DIR"                                                           \
         -Dassimp_DIR:PATH="$assimp_DIR"                                                           \
         -DCMAKE_BUILD_TYPE=$CONFIG                                                                \
      || fatal_error "failed to configure sequoia"
make -j2 || fatal_error "failed to build sequoia"
popd

# Test
pushd "$(pwd)"
cd build/sequoia-engine
ctest -C ${CONFIG}  -E "RenderGLTest" --output-on-failure --force-new-ctest-process                \
      || fatal_error "failed to run tests of sequoia"
popd
