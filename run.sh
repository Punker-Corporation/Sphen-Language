#!/bin/bash

ERROR() {
    echo -e "\e[41m E: $1 \e[0m"
}

clear
cd sphen || { ERROR "Directory sphen not found"; exit 1; }
echo "Building..."
if [ ! -d "build" ]; then
    mkdir build || { ERROR "Error to create build dir"; exit 1; }
fi

cd build || { ERROR "Error to access build dir"; exit 1; }

echo "Compiling with CMake..."
if [ ! -f "Sphen" ]; then
    cmake .. || { ERROR "CMake configuration failed"; exit 1; }
fi
make #VERBOSE=1

cd ../.. || { ERROR "Error returning to root directory"; exit 1; }

if [ ! -f "sphen/build/Sphen" ]; then
    { ERROR "Sphen executable not found or created"; exit 1; }
fi

./sphen/build/Sphen program/main.sp
