#!/bin/bash

[[ -d build/gcc-debug ]] || ./gcc-debug-init.sh
cd build/gcc-debug
cmake --build . --target move-mm-tests -j24 || exit