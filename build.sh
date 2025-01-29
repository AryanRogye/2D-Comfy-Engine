#!/bin/bash

OS=$(uname)                             # OS variable
cxx_flags="-std=c++11 -ObjC++ -g"       # C++ flags
output="engine"                         # Output file
warnings="-Wno-writable-strings"        # Warnings Flags
depreciation="-DGL_SILENCE_DEPRECATION" # Depreciation Flags

src_dir="src"
include_dir="-I$src_dir -I$src_dir/glad -I$src_dir/KHR" # Include Directories

if [[ "$OS" == "Darwin" ]]; then
    # MACOS BUILD
    echo "Building For MacOS"
    libs="-framework Cocoa -framework OpenGL"

    # This is commented out cuz glad is not used in this project at least for now

    # Compile Glad
    # clang $cxx_flags -c $src_dir/glad.c -o $src_dir/glad.o -I$src_dir -I$src_dir/glad -I$src_dir/KHR
    # Check for any errors in compilation of glad.c
    # if [[ $? -ne 0 ]]; then
    #     echo "Error compiling glad.c"
    #     exit 1
    # fi

    # Compile Engine
    clang++ $cxx_flags $include_dir                    \
            $src_dir/main.cpp                          \
            $src_dir/platform/platform_macos.mm        \
            -o $output $libs $warnings $depreciation
                # $src_dir/glad.o                           \

    echo "Build Complete"

elif [[ "$OS" == "Linux" ]]; then
    echo "Building For Linux"
elif [[ "$OS" == "Windows" ]]; then

    libs="-luser32"
    echo "Building For Windows"
    clang -std=c++11 src/main.cpp -o engine -lc++ $libs $warnings

else
    echo "Unknown OS"
    exit 1
fi
