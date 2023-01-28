@echo off
mkdir build\ninja-debug
pushd build\ninja-debug
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DMOVE_MEMORY_MANAGER_WITH_TESTS=on -GNinja ../../
    copy compile_commands.json ..\..\compile_commands.json
popd