#!/bin/bash

./gcc-debug-compile-tests.sh && ./build/gcc-debug/tests/move-mm-tests
cp ./build/gcc-debug/compile_commands.json ./