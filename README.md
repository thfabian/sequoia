Sequoia  <a target="_blank" href="http://semver.org">![Version][Version.Badge]</a> <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![Travis status][TravisCI.Badge]</a>
=======

## Introduction

Sequoia is a collection of different tools to create 3D games for Windows and Linux using C++, Python
and Lua -- The project is still in very early development!


## Project organization
The project is organized in a couple of subdirectories.

- The [sequoia-engine](sequoia-engine) directory contains the 3D game-engine.
- The [sequoia-cmake](sequoia-cmake) directory contains various CMake modules and other scripts 
  shared among all the Sequoia projects.

## Continuous Integration

Sequoia builds are run on ![Travis CI][TravisCI].

|  Toolchain         |                                                     Status                                            |
|:-------------------|:-----------------------------------------------------------------------------------------------------:|
| Linux GCC 5.4      | ![Build Status](https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/1)  |
| Linux GCC 6.1      | ![Build Status](https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/3)  |

## License
Please see [LICENSE.txt](LICENSE.txt).

<!-- Links -->
[TravisCI]: https://travis-ci.org/thfabian/sequoia
[TravisCI.Badge]: https://travis-ci.org/thfabian/sequoia.svg?branch=master
[Version.Badge]: https://badge.fury.io/gh/thfabian%2Fsequoia.svg
