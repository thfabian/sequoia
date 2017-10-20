Sequoia <br/> <a target="_blank" href="http://semver.org">![Version][Version.Badge]</a> <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![Travis status][TravisCI.Badge]</a>
=======

## Introduction

Sequoia is a collection of tools to create 3D games for Windows and Linux using C++, Python.
and Lua 

> The project is still in very early development!

## Project organization
The project is organized in a couple of subdirectories.

- The [sequoia-cmake](sequoia-cmake) directory contains various CMake modules and other scripts 
  shared among all the Sequoia projects.
- The [sequoia-docs](sequoia-docs) directory contains the HTML documentation of all Sequoia projects.
- The [sequoia-engine](sequoia-engine) directory contains the 3D game-engine.

## Continuous Integration

### Linux
|  Toolchain   | Config         |                                                     Status                                                        |
|:-------------|:---------------|:-----------------------------------------------------------------------------------------------------------------:|
| GCC 5.4      | Release        |  <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![GCC 5.4][GCC_54_Release.Badge]</a>            |
| GCC 5.4      | RelWithDebInfo |  <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![GCC 5.4][GCC_54_RelWithDebInfo.Badge]</a>     |
| GCC 6.3      | Release        |  <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![GCC 6.3][GCC_63_Release.Badge]</a>            |
| GCC 6.3      | RelWithDebInfo |  <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![GCC 6.3][GCC_63_RelWithDebInfo.Badge]</a>     |
| Clang 3.8    | Release        |  <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![Clang 3.8][Clang_38_Release.Badge]</a>        |
| Clang 3.8    | RelWithDebInfo |  <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![Clang 3.8][Clang_38_RelWithDebInfo.Badge]</a> |
| Clang 4.0    | Release        |  <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![Clang 4.0][Clang_40_Release.Badge]</a>        |
| Clang 4.0    | RelWithDebInfo |  <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![Clang 4.0][Clang_40_RelWithDebInfo.Badge]</a> |


## License

> You can check out the full license [here](LICENSE.txt).

This project is licensed under the terms of the **MIT** license.

<!-- Links -->
[TravisCI]: https://travis-ci.org/thfabian/sequoia
[TravisCI.Badge]: https://travis-ci.org/thfabian/sequoia.svg?branch=master
[Version.Badge]: https://badge.fury.io/gh/thfabian%2Fsequoia.svg
[GCC_54_Release.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/3
[GCC_54_RelWithDebInfo.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/4
[GCC_63_Release.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/5
[GCC_63_RelWithDebInfo.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/6
[Clang_38_Release.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/7
[Clang_38_RelWithDebInfo.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/8
[Clang_40_Release.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/9
[Clang_40_RelWithDebInfo.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/10
