##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##
unset(projects)
unset(ogre_dependencies)

#
# Boost
#
list(APPEND projects boost)
list(APPEND ogre_dependencies boost)
set(boost_min_version 1.58.0)
set(_v 60)
set(boost_download_version 1.${_v}.0)
set(boost_url "http://sourceforge.net/projects/boost/files/boost/1.${_v}.0/boost_1_${_v}_0.tar.gz/download")
set(boost_md5 "28f58b9a33469388302110562bdf6188")
set(boost_components thread date_time system atomic chrono filesystem program_options)

#
# GTest
#
list(APPEND projects gtest)
set(gtest_version "1.7.0")
set(gtest_url "https://github.com/google/googletest/archive/release-${gtest_version}.tar.gz")
set(gtest_md5 "4ff6353b2560df0afecfbda3b2763847")

#
# TBB
#
list(APPEND projects tbb)
list(APPEND ogre_dependencies tbb)
set(tbb_url "https://github.com/thfabian/tbb/archive/tbb44u4.tar.gz")
set(tbb_md5 "bfbeb805c91fcd32f858f8d9c26ad0e8")

#
# GLSL optimizer
#
set(glsl_optimizer_version "1.0")
set(glsl_optimizer_url "https://github.com/thfabian/glsl-optimizer/archive/v${glsl-optimizer_version}.tar.gz")
set(glsl_optimizer_md5 "34da9bc78d9aa72cf7eb57d9b7913ece")

#
# HLSL2GLSL
#
set(hlsl2glsl_version "1.0")
set(hlsl2glsl_url "https://github.com/thfabian/hlsl2glslfork/archive/v${hlsl2glsl_version}.tar.gz")
set(hlsl2glsl_md5 "1ceab1c1c463bc05b4672f88c7f780cf")

#
# OGRE Dependencies
#
list(APPEND ogre_dependencies ogredeps)
set(ogredeps_version "1.1")
set(ogredeps_url "https://github.com/thfabian/ogredeps/archive/v${ogredeps_version}.tar.gz")
set(ogredeps_md5 "6052d4b2a6e9b232283eb0902fde539c")

#
# OGRE
#
list(APPEND projects ogre)
set(ogre_version "1-9")
set(ogre_url "https://bitbucket.org/sinbad/ogre/get/v${ogre_version}.tar.gz")
set(ogre_md5 "002fd9ba23ea92aebbe561213991770f")
set(ogre_components)

