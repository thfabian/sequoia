*************
sequoia-cmake
*************

CMake modules of the Sequoia project. To use the functions and macros, add the ``sequoia-cmake`` directory as a sub-directory (this will add the correct paths to the ``CMAKE_MODULE_PATH``).

.. code-block:: cmake

  add_subdirectory(<sequoia-cmake>)

Each function and macro uses a `snake-case <https://en.wikipedia.org/wiki/Snake_case>`_ identifier and is defined in a spereate file using he corresponding `camel-case <https://en.wikipedia.org/wiki/Camel_case>`_ filename. For example, to use the function ``sequoia_my_function`` include the file ``SequoiaMyFunction``.

.. code-block:: cmake

  include(SequoiaMyFunction)
  sequoia_my_function()

Functions & Macros
~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 2

   /sequoia-cmake/SequoiaAddTargetCleanAll
   /sequoia-cmake/SequoiaGetScriptDir
   /sequoia-cmake/SequoiaIncludeGuard

Modules
~~~~~~~

Load settings for an external project via `find_package <https://cmake.org/cmake/help/v3.0/command/find_package.html>`_.