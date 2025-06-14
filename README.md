
# OpenGL-GE
Making a simulation engine in OpenGL

## Requirements
Most of the source code has been included in the repo, only the build dependencies need to be satisfied:
1. [Cmake](https://cmake.org/download/)
2. [MinGW](https://www.mingw-w64.org/downloads/)

## Installation

### Common:

```bash
git clone --recurse-submodules https://github.com/kshitijaucharmal/OpenGL-GE
cd OpenGL-GE
mkdir build
cd build
```
### Windows
```bash
cmake -G "MinGW Makefiles" ..
mingw32-make
mingw32-make run # to execute
```

### Linux / Mac OS
```bash
cmake -DPython_EXECUTABLE=$(which python3.10) ..
make
make run # to execute
```

Feel free to let me know any bugs or improvements !!
