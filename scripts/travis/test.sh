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

# Make sure C++ and C compilers are set correctly
export CXX=${CXX_COMPILER}
export CC=${C_COMPILER}

$CC --version
$CXX --version

# Setup dependencies
bash ${this_script_dir}/install.sh -i ${CACHE_DIR} -b cmake,boost,glbinding,opencv
cmake --version

# Build
pushd $(pwd)
mkdir build && cd build
cmake .. -DBOOST_ROOT:PATH=$BOOST_ROOT                                                             \
         -DGLBINDING_ROOT:PATH=$GLBINDING_ROOT                                                     \
         -DOpenCV_DIR:PATH=$OpenCV_DIR                                                             \
         -DCMAKE_BUILD_TYPE=$CONFIG
make -j2
popd

# Test
pushd $(pwd)
cd build/sequoia-engine
ctest -C ${CONFIG}  -E "GameTest|RenderGLTest" --output-on-failure --force-new-ctest-process
popd
