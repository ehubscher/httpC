# COMP445_lab_assignment

This repository contains the collective COMP 445 Lab Assignment(s).

1. To build this project, make sure that make, CMake, cmocka, and a C compiler are installed on your machine
(i.e. gcc, clang, llvm, etc.)
2. Next, within the project's root directory, create a directory and call it "build"; `mkdir build`.
3. Then, navigate to the build directory you just created; `cd build`.
4. Once in the build directory run; `cmake -G "Unix Makefiles" ..`. This will generate a Unix Makefile based off the
CMakeLists.txt file in the parent directory (i.e. the project's root directory).
5. Finally, you can run `make` to generate the packaged executable of the project.
