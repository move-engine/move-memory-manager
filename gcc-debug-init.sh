#!/bin/bash

mkdir -p build
mkdir -p build/gcc-debug

pushd build/gcc-debug
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DMOVE_MEMORY_MANAGER_WITH_TESTS=on ../../ && cp compile_commands.json ../../
popd