# Parse Server C++ SDK

## Prerequisites

- CMake, the cross-platform, open-source build system

- Doxygen, the de facto standard tool for generating documentation from annotated C++ sources

## Build

We use `cmake` to build our C++ SDK

- build ParseCloud C++ SDK

		$ cd cpp-sdk
		$ mkdir cpp-sdk-build
        $ cd cpp-sdk-build
		$ git submodule init
		$ git submodule update
        $ cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
        $ make -j4


### Docs

We can generate cpp api docs using doxygen.

		$ cd cpp-sdk
		$ doxygen Doxyfile

Then, a `docs` directory will be generated.

## High-level file hierarchy

- `include`: header files of ParseCloud

- `src`: source code of ParseCloud

- `tests`: unit test cases

- `scripts`: some useful scripts

- `lib`: third-party libraries

- `Doxyfile`: used to generate C++ docs

- `README.md`: brief introduction to this project

- `CMakeLists.txt`: cmake script which will be used to generate Makefile

## TODO
in the code
