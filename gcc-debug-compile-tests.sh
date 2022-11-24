#!/bin/bash

[[ -d build/gcc-debug ]] || ./gcc-debug-init.sh
cd build/gcc-debug
(cmake --build . --target move-mm-tests -j24 && cp compile_commands.json ../) || (cp compile_commands.json ../ && exit 1)
cp compile_commands.json ../