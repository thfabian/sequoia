# sequoia-cmake

This repository contains the CMake scripts shared among all the Sequoia projects. 

## Installation

Add `<sequoia-cmake>/cmake` to the `CMAKE_MODULE_PATH` to use the functions, macros and modules:

```cmake
list(APPEND CMAKE_MODULE_PATH "<sequoia-cmake>")
```

Note that all sequoia projects contain a `sequoia_cmake_init` macro which tries to find the sequoia-cmake module and respects the `SEQUOIA_CMAKE_DIR` (environment) variable.

```cmake
include(SequoiaCMakeInit)
sequoia_cmake_init()
```

## Usage 

Each function and macro uses a [snake-case](https://en.wikipedia.org/wiki/Snake_case>) identifier and is defined in a spereate file using the corresponding [camel-case](https://en.wikipedia.org/wiki/Camel_case>) filename. For example, to use the function `sequoia_add_target_clean_all` include the file `SequoiaAddTargetCleanAll`.

```cmake
include(SequoiaAddTargetCleanAll)
sequoia_add_target_clean_all()
```

## License

> You can check out the full license [here](LICENSE.txt).

This project is licensed under the terms of the **MIT** license.
