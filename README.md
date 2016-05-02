BBC Graphics Viewer
===================

BBC Graphics Viewer is a utility for viewing and converting graphics files
originally created on BBC Micro or Master computers.

Compile
-------

You'll need the following in addition to a C++ compiler:

* [CMake](https://cmake.org/) in your PATH
* The [Qt framework](https://www.qt.io/), version 5.x

First, open a shell in the root of the project and then run the following to
fetch the submodule and generate the build files.

``` sh
git submodule update --init
mkdir build
cd build
cmake ..
```

If CMake can't find your installation of Qt, you might need to set the
`CMAKE_PREFIX_PATH` environment variable.

Finally, you can open `build\Beebview.sln` (Windows) or run `make` (Linux) to build.

Contribute
----------

Please feel free to send pull requests to fix bugs or add enhancements.
