BBC Graphics Viewer
===================

A cross-platform viewer and converter for graphics files originally created on
BBC Micro or Master computers.

Two of the most common ways of storing images on the BBC were as a dump of the
graphics memory, or in a run-length encoded format loaded by a program called
[LdPic](https://nerdoftheherd.com/projects/libbeebimage/ldpic/).

However, due to the unusual way that the BBC's graphics memory was mapped to
the screen (in blocks of eight bytes) it is not straight-forward to view either
of these formats on a PC with other tools.

This utility is able to display BBC graphics files saved in LdPic or memory
dump format in Modes 0, 1, 2, 4 and 5. It can also save any of the images it
displays in a variety of image formats.

Runs under both Windows and Linux (requires the [Qt](https://www.qt.io/)
framework).

Compile
-------

You'll need the following in addition to a C++ compiler:

* [CMake](https://cmake.org/) in your PATH
* The [Qt framework](https://www.qt.io/), version 6.x

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
