Sequoia  <a target="_blank" href="http://semver.org">![Version][Version.Badge]</a> <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![Travis status][TravisCI.Badge]</a>
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
|  Toolchain   |                                                     Status                                     |
|:-------------|:----------------------------------------------------------------------------------------------:|
| GCC 5.4      | <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![GCC 5.4][GCC_54.Badge]</a>  |
| GCC 6.3      | <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![GCC 6.3][GCC_63.Badge]</a>  |
| Clang 4.0    | <a target="_blank" href="https://travis-ci.org/thfabian/sequoia">![Clang 4.0][Clang_40.Badge]</a>  |


## License

> You can check out the full license [here](LICENSE.txt).

This project is licensed under the terms of the **MIT** license.

<!-- Links -->
[TravisCI]: https://travis-ci.org/thfabian/sequoia
[TravisCI.Badge]: https://travis-ci.org/thfabian/sequoia.svg?branch=master
[Version.Badge]: https://badge.fury.io/gh/thfabian%2Fsequoia.svg
[GCC_54.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/3
[GCC_63.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/5
[Clang_40.Badge]: https://travis-matrix-badges.herokuapp.com/repos/thfabian/sequoia/branches/master/7
