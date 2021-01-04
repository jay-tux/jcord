#!/bin/bash

echo " --- Attempting to move to root directory... --- "
cd .. #move to root directory
echo " --- At root (I hope): $PWD --- "

echo " --- Attempting to detect libraries... --- "
if [ ! -d "libs/" ]; then
    mkdir "libs/"
fi

if [ ! -d "libs/sleepy-discord" ]; then
    echo "Please enter the location of the Sleepy-Discord library:"
    read -p sleepy
    cp "${sleepy}" "libs/" -r
    if [ $? != 0 ]; then
        echo "Failed to copy. Exiting"
        exit 1
    fi
fi

if [ ! -d "libs/ClipboardXX" ]; then
    echo "Please enter the location of the ClipboardXX library:"
    read -p clip
    cp "${clip}" "libs/" -r
    if [ $? != 0 ]; then
        echo "Failed to copy. Exiting"
        exit 1
    fi
fi

echo "#include <ncurses.h>
int main() {}" | gcc -x c -lncurses -o tmp.out -

if [ $? != 0 ]; then
    echo "Please install ncurses and/or GCC before compiling JCord. Exiting"
    exit 1
else
    rm tmp.out
fi

echo " --- All libraries copied/detected! --- "

echo " --- Attempting to detect required tools... --- "
if [ ! command -v cmake ]; then
    echo "CMake could not be found. Exiting"
    exit 1
fi

if [ ! command -v make ]; then
    echo "Make could not be found. Exiting"
    exit 1
fi
echo " --- Build tools found! --- "

echo " --- Preparing and building... --- "
if [ ! -d "out/" ]; then
    mkdir "out/"
fi

cd "out/"
cmake ..
if [ $? != 0 ]; then
    echo "Failed to run CMake. Please report any bugs on https://github.com/jay-tux/jcord/"
    exit 2
fi

make jcord
if [ $? != 0 ]; then
    echo "Failed to compile. Please check the guide and/or report any bugs on https://github.com/jay-tux/jcord/"
    exit 2
fi
echo " --- JCord has been built! --- "
