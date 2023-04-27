@echo off

if not exist build\ninja-debug (
    call ninja-debug-init.bat
)

pushd build\ninja-debug
    cmake --build . --target move-mm-tests || (
        copy compile_commands.json ..\..\compile_commands.json
        exit /B 1
    )
    copy compile_commands.json ..\..\compile_commands.json
    
    @REM mkdir ..\..\bin
    @REM copy bin\move-mm-tests* ..\..\bin\
    @REM copy bin\*.dll ..\..\bin\
popd