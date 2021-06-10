@echo off
cd %~dp0
call var.bat

set PRJ_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\prj.emscripten-static"

if not exist %PRJ_DIR% (
    mkdir %PRJ_DIR%
)
cd %PRJ_DIR%


call emsdk_env.bat
set EMSCRIPTEN=%EMSDK%/upstream/emscripten/


set CMAKELISTS_DIR=%~dp0..\..\
cmake %CMAKELISTS_DIR% -G "MinGW Makefiles"                                    ^
    -DCMAKE_TOOLCHAIN_FILE=%MAZE_ENGINE_DIR%\cmake\toolchains\Emscripten.cmake     ^
    -DBUILD_SHARED_LIBS=OFF                                    

pause
